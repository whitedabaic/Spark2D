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
				rotation = 90
			},
			sprite = {
				asset_name = "ship",
				width = 103,
				height = 84,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 42
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
				width = 13,
				height = 37,
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