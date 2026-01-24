AsteroidDefs = 
{
	asteroid_big =
	{
		group = "asteroids",
		components =
		{
			transform = { 
				position = { x = -100, y = -100},
				scale = { x = 1, y = 1},
				rotation = 0
			},
			sprite = {
				asset_name = "ast_1",
				width = 120,
				height = 98,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 59
			}
		}, 
		type = "big",
		min_speed = 1,
		max_speed = 3,
	},
	asteroid_small =
	{
		group = "asteroids",
		components =
		{
			transform = { 
				position = { x = -100, y = -100},
				scale = { x = 1, y = 1},
				rotation = 0
			},
			sprite = {
				asset_name = "ast_2",
				width = 28,
				height = 28,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 14
			}
		},
		type = "small",
		min_speed = 1,
		max_speed = 2,
	},
}

ShipDefs = 
{
	blue_ship =
	{
		tag = "ship",
		components =
		{
			transform = { 
				position = { x = 320, y = 240},
				scale = { x = 1, y = 1},
				rotation = 0
			},
			sprite = {
				asset_name = "ship",
				width = 84,
				height = 104,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 42
			},
			animation = {
				num_frames = 0,
				frame_rate = 0,
				frame_offset = 0,
				bVertical = false,
				bLooped = false
			}
		}
	}
}

ProjectileDefs =
{
	proj_1 =
	{
		group = "projectiles",
		components =
		{
			transform = { 
				position = { x = 0, y = 0},
				scale = { x = 1, y = 1},
				rotation = 0
			},
			sprite = {
				asset_name = "proj_1",
				width = 37,
				height = 13,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 10
			}
		},
		life_time = 2000,
		proj_speed = 10
	}
}

HudDefs =
{
	lives = 
	{
		group = "lives",
		components =
		{
			transform = { 
				position = { x = 0, y = 0},
				scale = { x = 1, y = 1},
				rotation = 0
			},
			sprite = {
				asset_name = "lives",
				width = 37,
				height = 26,
				start_x = 0, start_y = 0,
				layer = 10
			}
		}
	},
	score = 
	{
		group = "score",
		components =
		{
			transform = { 
				position = { x = 200, y = 0},
				scale = { x = 4, y = 4},
				rotation = 0
			},
			sprite = {
				asset_name = "score",
				width = 48,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 10
			}
		}
	},
	number = 
	{
		group = "numbers",
		components =
		{
			transform = { 
				position = { x = 380, y = 0},
				scale = { x = 4, y = 4},
				rotation = 0
			},
			sprite = {
				asset_name = "numbers",
				width = 16,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 10
			}
		}
	},
	game_over = 
	{
		tag = "game_over",
		components =
		{
			transform = { 
				position = { x = (WINDOW_WIDTH / 2) - 240, y = (WINDOW_HEIGHT / 2) - 48},
				scale = { x = 6, y = 6},
				rotation = 0
			},
			sprite = {
				asset_name = "game_over",
				width = 80,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 10,
				hidden = true
			}
		}
	},
}