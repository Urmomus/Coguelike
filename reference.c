int main(void)
{
	{ // иван
		map = generate_map(parameters);
	}
	
	{ // Коля
		items = generate_items();
		mobs = generate_mobs();
		player = generate_player();
	}
	
	{ // иван
		place(map, mobs, MOBS);
		place(map, items, ITEMS);
		place(map, player, PLAYER);
	}
	
	end_of_game = false;
	while (!end_of_game)
	{
		{ // Дима
			action = input_from_keyboard();
		}
		
		bool renew_map;
		
		{ // Коля
			do_action(player, action, &renew_map);
		}
		
		{ // Мэтью
		renew_screen();
		}
		if (!renew_map)
			continue;
		
		{ // Коля | Ваня
			renew_map_state(map);
			renew_mobs_state(map);	// игрок считается мобом
			renew_items_state(map);
		
			end_of_game = check_if_the_end(player);
		}
	};
	
	{ // Мэтью
		print_result_of_game(player);
	}
};