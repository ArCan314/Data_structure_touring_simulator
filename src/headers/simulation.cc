#ifndef SRC_SIMULATE_CC
#define SRC_SIMULATE_CC

#pragma execution_character_set("utf-8")

#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <memory>

#include "simulation.h"
#include "io.h"
#include "traveller.h"
#include "city_graph.h"
#include "user_type.h"
#include "time_format.h"
#include "path.h"
#include "log.h"
#include "id_map.h"

#if defined(_WIN32) || (defined(__CYGWIN__) && !defined(_WIN32)) || defined(__MINGW32__) || defined(__MINGW64__)
#include <Windows.h>
#endif

static Time current_time;
static int sleep_millsecs = 500;

// 返回每两次调用间的间隔毫秒数
std::chrono::duration<double, std::milli> Timer()
{
    static int count = 0;
    static std::chrono::time_point<std::chrono::system_clock> start_time;
    static std::chrono::time_point<std::chrono::system_clock> end_time;
    if (count % 2)
    {
        end_time = std::chrono::system_clock::now();
        count++;
        return end_time - start_time;
    }
    else
    {

        start_time = std::chrono::system_clock::now();
        count++;
        return start_time - start_time;
    }
}

// 接受一个值,根据值的奇偶来返回时间间隔,当count为奇数时返回该次调用与上次count为偶数时调用的时间间隔
std::chrono::duration<double, std::milli> Timer(int count)
{
    static std::chrono::time_point<std::chrono::system_clock> start_time;
    static std::chrono::time_point<std::chrono::system_clock> end_time;
    if (count % 2)
    {
        end_time = std::chrono::system_clock::now();
        return end_time - start_time;
    }
    else
    {
        start_time = std::chrono::system_clock::now();
        return start_time - start_time;
    }
}

void setSleepMillsecs(int sleep_ms)
{
    sleep_millsecs = sleep_ms;
}

void SleepFor(int millseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_millsecs - millseconds));
}

void InitializeSimulator(const Time &start_time)
{
    current_time = start_time;
}

#if defined(_WIN32) || (defined(__CYGWIN__) && !defined(_WIN32)) || defined(__MINGW32__) || defined(__MINGW64__)

static HANDLE InitOutBuf()
{
    HANDLE hTempOutBuf = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);
    CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hTempOutBuf, &cci);
    cci.bVisible = 0;
    SetConsoleCursorInfo(hTempOutBuf, &cci);
    return hTempOutBuf;
}

void Simulate(Traveller &traveller, const CityGraph &city_graph, const IDMap &id_map)
{
    Log::LogWrite("模拟开始");
    static bool out_buf_inited = false;
    static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    static HANDLE hOutBuf;

    if (!out_buf_inited)
    {
        hOutBuf = InitOutBuf();
        out_buf_inited = true;
    }

    SetConsoleActiveScreenBuffer(hOutBuf);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    SMALL_RECT srctRect = csbi.srWindow;
    COORD coordBufSize = csbi.dwSize;
    COORD coordBufCoord = {0, 0};
    CHAR_INFO *data_buffer = new CHAR_INFO[coordBufSize.X * coordBufSize.Y];

    int count = 0;
    while (traveller.get_position() != -1)
    {
        Timer(count++);
        ClearScreen();

        Path path = traveller.get_path();
        int position = traveller.get_position();

        City_id current_city = path.getNode(position).former_city;
        City_id next_city = path.getNode(position).current_city;
        int route_index = path.getNode(position).kth_way;
        // Trans_id transport_type = city_graph.get_route(current_city, next_city, route_index).transport_type;
        std::cout << "当前时间 : " << current_time.get_day() << " 日 " << current_time.get_hour() << "时" << std::endl;
        //PrintPath(city_graph, id_map, traveller.get_path(), traveller.get_position());
        PrintTravellerInfo(city_graph, id_map, current_time, traveller);

        ReadConsoleOutput(hStdOut, data_buffer, coordBufSize, coordBufCoord, &srctRect);
        WriteConsoleOutput(hOutBuf, data_buffer, coordBufSize, coordBufCoord, &srctRect);

        traveller.updateState(city_graph);
        current_time.add_time(1);
        auto duration = Timer(count++);
        // std::cout << duration.count();
        SleepFor(duration.count());
        // std::cin >> click;
    }
    ClearScreen();
    std::cout << "当前时间 : " << current_time.get_day() << " 日 " << current_time.get_hour() << "时" << std::endl;
    std::cout << "到达目的地" << std::endl;
    PrintPath(city_graph, id_map, traveller.get_path(), traveller.get_path().get_len());

    SetConsoleActiveScreenBuffer(hStdOut);
    delete[] data_buffer;
    Log::LogWrite("模拟结束");
}

#else

void Simulate(Traveller &traveller, const CityGraph &city_graph, const IDMap &id_map)
{
    Log::LogWrite("模拟开始");
    int count = 0;

    while (traveller.get_position() != -1)
    {
        //char click;
        Timer(count++);
        ClearScreen();

        Path path = traveller.get_path();
        int position = traveller.get_position();

        City_id current_city = path.getNode(position).former_city;
        City_id next_city = path.getNode(position).current_city;
        int route_index = path.getNode(position).kth_way;
        Trans_id transport_type = city_graph.get_route(current_city, next_city, route_index).transport_type;
        std::cout << "当前时间 : " << current_time.get_day() << " 日 " << current_time.get_hour() << "时" << std::endl;
        //PrintPath(city_graph, id_map, traveller.get_path(), traveller.get_position());
        PrintTravellerInfo(city_graph, id_map, current_time, traveller);
        traveller.updateState(city_graph, current_time);
        current_time.add_time(1);
        auto duration = Timer(count++);
        // std::cout << duration.count();
        SleepFor(duration.count());
        // std::cin >> click;
    }

    ClearScreen();
    std::cout << "当前时间 : " << current_time.get_day() << " 日 " << current_time.get_hour() << "时" << std::endl;
    std::cout << "到达目的地" << std::endl;
    PrintPath(city_graph, id_map, traveller.get_path(), traveller.get_path().get_len());
    Log::LogWrite("模拟结束");
}
#endif

#endif // SRC_SIMULATE_CC
