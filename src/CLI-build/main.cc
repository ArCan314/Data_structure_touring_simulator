#ifndef TEST_MAIN
#define TEST_MAIN

#pragma execution_character_set("utf-8")

#include <iostream>
#include <thread>

#include <stdlib.h>

#include "../src/headers/user_type.h"
#include "../src/headers/io.h"
#include "../src/headers/id_map.h"
#include "../src/headers/city_graph.h"
#include "../src/headers/traveller.h"
#include "../src/headers/time_format.h"
#include "../src/headers/log.h"
#include "../src/headers/path.h"
#include "../src/headers/simulation.h"
#include "../src/headers/save_at_exit.h"

int main()
{
	system("chcp 65001");
	IDMap id_map;
	CityGraph city_graph;
	Traveller traveller;
	Path path;
	std::vector<City_id> plan;
	Strategy strategy;
	Time limit_time;
	Time init_time;

	eSettings settings_option;

	ClearScreen();
	int account_name_line = Welcome(traveller);
	ClearScreen();

	traveller.loadData(account_name_line, city_graph);
	setTravellerPtr(&traveller);
	setSignalHandlers();
	while (1)
	{
		int opcode = Menu(id_map, traveller);

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		switch (opcode)
		{
		case SCHEDULE:
			plan = Request(id_map);
			traveller.set_plan(plan);
			// traveller.ShowPlan();
			strategy = InputStrategy(init_time, limit_time);

			path = traveller.schedulePath(city_graph, strategy, init_time, limit_time);

			// path.Show();
			PrintPath(city_graph, id_map, path);
			if (PathConfirm())
				traveller.set_path(path);
			ClearScreen();
			break;
		case INQUIRE_STATE:
			std::cout << "用户名: " << traveller.get_ID() << std::endl;
			PrintTravellerInfo(city_graph, id_map, traveller.get_init_time(), traveller);
			break;
		case INQUIRE_PATH:
			//PrintPath(city_graph, id_map, path, 0);
			PrintRoutes(city_graph, id_map);
			break;
		case SIMULATE:
			traveller.initState(city_graph);
			InitializeSimulator(traveller.get_init_time());
			Simulate(traveller, city_graph, id_map);
			break;
		case SETTINGS:
			ClearScreen();
			settings_option = SettingsMenu();
			switch (settings_option)
			{
			case SIMULATION_SPEED:
				setSleepMillsecs(getSimulateSpeed());
				break;
			case CONSOLE_FONT_SIZE:
				SetConsoleFontSize();
			}

			break;
		case EXIT:
			std::exit(0);
			break;
		default:
			break;
		}
	}
	Log::CloseLog();
	return 0;
}
/*

*/
#endif // TEST_MAIN