struct Button_State {
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_W,
	BUTTON_S,
	BUTTON_A,
	BUTTON_D,
	BUTTON_P,
	BUTTON_ENTER,
	BUTTON_ESCAPE,

	BUTTON_COUNT,
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};