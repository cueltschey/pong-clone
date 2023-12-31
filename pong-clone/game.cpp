
#define is_down(b) input->buttons[b].is_down
#define is_pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define is_released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y = 0, ball_half_size = 1;
int score_p_1 = 0, score_p_2 = 0;

internal void
simulate_player(float &p, float &dp, float &ddp, float dt) {
	ddp -= dp * 10.f;

	p = p + dp * dt + ddp * dt * dt * .5f;
	dp = dp + ddp * dt;

	if (p + player_half_size_y > arena_half_size_y) {
		p = arena_half_size_y - player_half_size_y;
		dp = 0;
	}
	else if (p - player_half_size_y < -arena_half_size_y) {
		p = -arena_half_size_y + player_half_size_y;
		dp = 0;
	}
}

static void 
simulate_game(Input* input, float dt) {
	clear_screen(0x00000);
	draw_rect(0, 0, 85, 45, 0x343434);

	float player_1_ddp = 0.f;
	if (is_down(BUTTON_UP)) player_1_ddp += 2000;
	if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;

	float player_2_ddp = 0.f;
	if (is_down(BUTTON_W)) player_2_ddp += 2000;
	if (is_down(BUTTON_S)) player_2_ddp -= 2000;

	simulate_player(player_1_p, player_1_dp, player_1_ddp, dt);
	simulate_player(player_2_p, player_2_dp, player_2_ddp, dt);

	ball_p_x += ball_dp_x * dt;
	ball_p_y += ball_dp_y * dt;
	ball_dp_x *= 1.0003;

	draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);

	if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
		ball_p_x - ball_half_size < 80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
		ball_p_y - ball_half_size < player_1_p + player_half_size_y) {
		ball_p_x = 80 - player_half_size_x - ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = player_1_dp * 0.75f;
	}else if (ball_p_x + ball_half_size >- 80 - player_half_size_x &&
		ball_p_x - ball_half_size < -80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
		ball_p_y + ball_half_size < player_2_p + player_half_size_y) {
		ball_p_x = -80 + player_half_size_x + ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = player_2_dp * 0.75f;
	}

	if (ball_p_y + ball_half_size > arena_half_size_y) {
		ball_dp_y *= -1;
		ball_p_y = arena_half_size_y - ball_half_size;
	}
	else if (ball_p_y - ball_half_size < -arena_half_size_y) {
		ball_dp_y *= -1;
		ball_p_y = -arena_half_size_y + ball_half_size;
	}

	if (ball_p_x + ball_half_size > arena_half_size_x) {
		ball_dp_x *= -1;
		ball_dp_y = 0; ball_p_x = 0; ball_p_y = 0;
		score_p_2++;
		ball_dp_x = 100;
	}
	else if (ball_p_x - ball_half_size < -arena_half_size_x) {
		ball_dp_x *= -1;
		ball_dp_y = 0; ball_p_x = 0; ball_p_y = 0;
		score_p_1++;
		ball_dp_x = 100;
	}

	draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xffffff);
	draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xffffff);

	if (score_p_1 >= 10 || score_p_2 >= 10) running = false;

	for (int i = 0; i < score_p_1; ++i) {
		draw_rect(80 - (i * 5), 47, 1, 1, 0xffffff);
	}
	for (int j = 0; j < score_p_2; ++j) {
		draw_rect(-80 + (j * 5), 47, 1, 1, 0xffffff);
	}
}