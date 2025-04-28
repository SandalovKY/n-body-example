#include <windows.h>
#include <vector>
#include <cstdlib>
#include "GSimulation.hpp"

const char g_szClassName[] = "MyWindowClass";

HWND g_hwnd;
std::vector<POINT> points;

HANDLE hWorkerThread = NULL; // Дескриптор потока
bool running = true;  // Флаг для остановки потока

// Структура для передачи данных между потоками
struct UpdateData {
    std::vector<POINT> newPoints;
};

struct ThreadData {
    int body_num;
    int iters;
};

std::atomic_bool drop = false;

// Функция, которую выполняет рабочий поток
DWORD WINAPI WorkerThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;

    GSimulation sim;
    sim.set_number_of_particles(data->body_num);
    sim.set_number_of_steps(data->iters);
    sim.start([&]{
        if (drop) {
            sim.have_to_drop();
        } else {
            std::vector<POINT> updatedPoints;
            size_t parts_num = sim.get_part_num();
            Particle* parts = sim.get_parts();
            real_type area_lim = sim.get_area_lim();
            for(size_t i = 0; i < parts_num; ++i) {
                if (parts[i].pos[0] < area_lim && parts[i].pos[1] < area_lim) {
                        updatedPoints.push_back({(int)((parts[i].pos[0] / area_lim) * 600), (int)((parts[i].pos[1] / area_lim) * 600)});
                        // updatedPoints.push_back({(200), (200)});
                    }
            }
            UpdateData* updateData = new UpdateData{updatedPoints};
            PostMessage(g_hwnd, WM_USER + 1, (WPARAM)updateData, 0);
            Sleep(10);
        }
    });
    return 0;
}

// Обработчик оконных сообщений
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Очищаем окно (рисуем фон белым цветом)
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));

            // Отрисовываем точки
            for (const auto& pt : points) {
                Rectangle(hdc, pt.x - 2, pt.y - 2, pt.x + 2, pt.y + 2);
            }

            EndPaint(hwnd, &ps);
        }
        break;

        case WM_DESTROY:
            drop = true;
            running = false; // Останавливаем рабочий поток
            WaitForSingleObject(hWorkerThread, INFINITE); // Ждем завершения потока
            CloseHandle(hWorkerThread); // Закрываем дескриптор потока
            PostQuitMessage(0);
        break;

        case WM_USER + 1: {
            // Обработка данных из рабочего потока
            UpdateData* updateData = (UpdateData*)wParam;
            points = updateData->newPoints;
            delete updateData; // Освобождаем память
            InvalidateRect(hwnd, NULL, FALSE); // Перерисовываем окно
        }
        break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    g_hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Moving Points in Parallel",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
        NULL, NULL, hInstance, NULL);

    if(g_hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    int argc;
    wchar_t **argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    
    if (argv == NULL) {
        std::cerr << "Error parsing command line" << std::endl;
        return 1;
    }

    // Инициализация точек
    int pointCount = 1000;
    int iters = 10;
    wchar_t* endPtr;
    if (argc != 3) {
        std::cout << "Use: n_body_simulation.exe PARTICLES_NUM STEPS_NUM\n";
        return 0;
    }
    pointCount = std::wcstol(argv[1], &endPtr, 10);
    iters = std::wcstol(argv[2], &endPtr, 10);

    ThreadData data{pointCount, iters};

    // Запускаем рабочий поток
    hWorkerThread = CreateThread(NULL, 0, WorkerThread, &data, 0, NULL);
    if (hWorkerThread == NULL) {
        MessageBox(NULL, "Failed to create worker thread", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}