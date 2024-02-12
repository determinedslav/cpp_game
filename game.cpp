#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float enemy_x = -80;
float player_x = 80;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2, player_half_size_y = 12;
float ball_half_size = 1;

float text_size = 1.f;

enum Gamemode {
	GM_MENU,
	GM_SETTINGS,
	GM_GAMEPLAY
};

Gamemode current_gamemode = GM_MENU;
int selected_players;
int selected_settings;
bool is_paused = false;
bool enemy_is_ai;

float ai_base_speed = 333.3f;
int difficulty = 2;

float ball_base_speed = 50.f;
int ball_speed_modifier = 2;

float player_y, player_dp, enemy_y, enemy_dp;
float ball_p_x, ball_p_y, ball_dp_x = ball_base_speed, ball_dp_y;

int player_score, enemy_score;

internal void
simulate_player(float* p, float* dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

internal void
reset_ball() {
	ball_dp_x *= -1;
	ball_dp_y = 0;
	ball_p_x = 0;
	ball_p_y = 0;
}

internal void
reset_game() {
	current_gamemode = GM_MENU;
	player_y = player_dp = enemy_y = enemy_dp = 0;
	ball_p_x = ball_p_y = ball_dp_y = 0;
	player_score = enemy_score = 0;
}

internal void
simulate_game(Input* input, float dt) {
	if (!is_paused) {
		draw_rect(0, 0, arena_half_size_x, arena_half_size_y, color_arena);
		draw_arena_borders(arena_half_size_x, arena_half_size_y, color_borders);

		if (pressed(BUTTON_ESCAPE)) {
			reset_game();
		}

		if (current_gamemode == GM_GAMEPLAY) {
			if (pressed(BUTTON_P)) {
				is_paused = !is_paused;
			}
			float player_ddp = 0.f;
			float enemy_ddp = 0.f;
			int ai_speed = ai_base_speed * difficulty;

			if (is_down(BUTTON_UP)) player_ddp += 1000;
			if (is_down(BUTTON_DOWN)) player_ddp -= 1000;

			if (!enemy_is_ai) {
				if (is_down(BUTTON_W)) enemy_ddp += 1000;
				if (is_down(BUTTON_S)) enemy_ddp -= 1000;
			}
			else {
				enemy_ddp = (ball_p_y - enemy_y) * 100;
				if (enemy_ddp > ai_speed) enemy_ddp = ai_speed;
				if (enemy_ddp < -ai_speed) enemy_ddp = -ai_speed;
			}

			simulate_player(&player_y, &player_dp, player_ddp, dt);
			simulate_player(&enemy_y, &enemy_dp, enemy_ddp, dt);

			{
				ball_p_x += ball_dp_x * dt;
				ball_p_y += ball_dp_y * dt;

				if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, player_x, player_y, player_half_size_x, player_half_size_y)) {
					ball_p_x = player_x - player_half_size_x - ball_half_size;
					ball_dp_x *= -1;
					ball_dp_y = (ball_p_y - player_y) * 2 + player_dp * .75f;
				}
				else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, enemy_x, enemy_y, player_half_size_x, player_half_size_y)) {
					ball_p_x = enemy_x + player_half_size_x + ball_half_size;
					ball_dp_x *= -1;
					ball_dp_y = (ball_p_y - enemy_y) * 2 + enemy_dp * .75f;
				}

				if (ball_p_y + ball_half_size > arena_half_size_y) {
					ball_p_y = arena_half_size_y - ball_half_size;
					ball_dp_y *= -1;
				}
				else if (ball_p_y - ball_half_size < -arena_half_size_y) {
					ball_p_y = -arena_half_size_y + ball_half_size;
					ball_dp_y *= -1;
				}

				if (ball_p_x + ball_half_size > arena_half_size_x) {
					reset_ball();
					enemy_score++;
				}
				else if (ball_p_x - ball_half_size < -arena_half_size_x) {
					reset_ball();
					player_score++;
				}
			}

			draw_rect(0, 0, 1, arena_half_size_y, color_score);
			draw_number(enemy_score, -9, 43, text_size, color_score);
			draw_number(player_score, 6, 43, text_size, color_score);

			draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, color_white);

			draw_rect(enemy_x, enemy_y, player_half_size_x, player_half_size_y, color_white);
			draw_rect(player_x, player_y, player_half_size_x, player_half_size_y, color_white);
		}
		else if (current_gamemode == GM_SETTINGS) {
			if (pressed(BUTTON_UP) || pressed(BUTTON_W)) {
				if ((!selected_settings && enemy_is_ai) && (difficulty < 3)) {
					difficulty++;
				}
				else if ((selected_settings || !enemy_is_ai) && (ball_speed_modifier < 3)) {
					ball_speed_modifier++;
				}
			}

			if (pressed(BUTTON_DOWN) || pressed(BUTTON_S)) {
				if ((!selected_settings && enemy_is_ai) && (difficulty > 0)) {
					difficulty--;
				}
				else if ((selected_settings || !enemy_is_ai) && (ball_speed_modifier > 1)) {
					ball_speed_modifier--;
				}
			}

			if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT) || pressed(BUTTON_A) || pressed(BUTTON_D)) {
				selected_settings = !selected_settings;
			}

			if (pressed(BUTTON_ENTER)) {
				current_gamemode = GM_GAMEPLAY;
			}

			if (enemy_is_ai) {
				draw_text(text_ai_level, -59, 18, text_size, color_text);
				if (!selected_settings) {
					draw_rect(-45, -10, 15, 20, color_borders);
				}
				draw_option_bars(-45, 0, difficulty);
				//draw_rect(-45, 0, 10, 4, difficulty > 2 ? color_button : color_button_unfocused);
				//draw_rect(-45, -10, 10, 4, difficulty > 1 ? color_button : color_button_unfocused);
				//draw_rect(-45, -20, 10, 4, difficulty > 0 ? color_button : color_button_unfocused);
			}

			draw_text(text_ball_speed, 27, 18, text_size, color_text);
			if (selected_settings || !enemy_is_ai) {
				draw_rect(45, -10, 15, 20, color_borders);
			}
			draw_option_bars(45, 0, ball_speed_modifier);
			//draw_rect(45, 0, 10, 4, ball_speed_modifier > 2 ? color_button : color_button_unfocused);
			//draw_rect(45, -10, 10, 4, ball_speed_modifier > 1 ? color_button : color_button_unfocused);
			//draw_rect(45, -20, 10, 4, ball_speed_modifier > 0 ? color_button : color_button_unfocused);

			ball_dp_x = ball_base_speed * ball_speed_modifier;
		}
		else if (current_gamemode == GM_MENU) {
			if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT) || pressed(BUTTON_A) || pressed(BUTTON_D)) {
				selected_players = !selected_players;
			}

			if (pressed(BUTTON_ENTER)) {
				current_gamemode = GM_SETTINGS;
				enemy_is_ai = selected_players ? 0 : 1;
			}

			//
			draw_text(text_alphabet, -80, 40, text_size, color_text);
			//

			draw_rect(-20, 0, 10, 10, selected_players ? color_button_unfocused : color_button);
			draw_text(text_1_player, -23, 2, text_size, selected_players ? color_text_unfocused : color_text);

			draw_rect(20, 0, 10, 10, selected_players ? color_button : color_button_unfocused);
			draw_text(text_2_player, 17, 2, text_size, selected_players ? color_text : color_text_unfocused);
		}
	} else {
		draw_pause_icon();

		if (pressed(BUTTON_P)) {
			is_paused = !is_paused;
		}
	}
}