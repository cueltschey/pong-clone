
#define is_down(b) input->buttons[b].is_down
#define is_pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define is_released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_pos_x = 0.f;
float player_pos_y = 0.f;

internal void
simulate_game(Input* input, float dt) {
	float speed = 50.f;
	clear_screen(0xff5500);
	if (is_down(BUTTON_UP)) player_pos_y += dt * speed;
	if (is_down(BUTTON_DOWN)) player_pos_y -= dt * speed;
	if (is_down(BUTTON_LEFT)) player_pos_x -= dt * speed;
	if (is_down(BUTTON_RIGHT)) player_pos_x += dt * speed;
	
	
	draw_rect(player_pos_x, player_pos_y, 1, 1, 0x00ff22);
}