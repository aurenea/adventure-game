#include <cmath>
#include <iostream>
#include "state.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

using namespace std;


ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* event_timer;

void error_message(const char* message) {
    unsigned long int code = 0;
    for (unsigned int i = 0; i < sizeof(message); i++) {
        code = (code+(unsigned int)((3+(5*i)+(7*pow(i, 2))+(11*pow(i, 3)))*pow((int)(message[i]), 2)))%0xfffffff;
        // TODO edit error code function
    }
    char error[17];
    strcat(error, "Error: 0x0000000");
    for (int i = 7; i > 0; i--) {
        unsigned long int p = 1;
        for (int j = 0; j < i; j++) { p *= 16; }
        if (code > p) {
            int intchar = (int)(code/p);
            error[15-i] = (char)(intchar+48+(intchar > 9 ? 7 : 0));
            code = code%p;
        }
    }
    al_show_native_message_box(display, error, message,
                               "The program has encountered a fatal error and will now close.",
                               NULL, ALLEGRO_MESSAGEBOX_ERROR);
}


State* state;

void set_state(State* s) {
    delete state;
    state = s;
}


void init() {
    display = NULL;
    event_queue = NULL;
    event_timer = NULL;

    if (!al_init()) {
        throw "Allegro library failed to initialize.";
    }

    if (!al_install_mouse()) {
        throw "Allegro mouse addon failed to initialize.";
    }

    if (!al_install_keyboard()) {
        throw "Allegro keyboard addon failed to initialize.";
    }

    if (!al_init_image_addon()) {
        throw "Allegro image addon failed to initialize.";
    }

    if (!al_init_font_addon()) {
        throw "Allegro font addon failed to initialize.";
    }

    if (!al_init_ttf_addon()) {
        throw "Allegro ttf addon failed to initialize.";
    }

    event_timer = al_create_timer(1.0/30);
    if (!event_timer) {
        throw "Allegro timer failed to initialize.";
    }

    display = al_create_display(640, 400);
    if (!display) {
        throw "Allegro display failed to initialize.";
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        throw "Allegro event queue failed to initialize.";
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(event_timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_hide_mouse_cursor(display);

    set_state(new MainMenuState(new InputController()));
}

void run() {
    al_start_timer(event_timer);

    ALLEGRO_EVENT* e;
    RunResult result = RunResult::CONTINUE;
    while (result >= RunResult::CONTINUE) {
        al_wait_for_event(event_queue, e);
        if (e->type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return;
        } else {
            result = state->run(e);
        }
    }
    return;
}


int main() {
    try {
        init();
        run();
        return 0;
    } catch (const char* c) {
        error_message(c);
        return 1;
    }
    return 1;
}

/*
void load_options();
void load_assets();
void load_objects();

int get_FPS();
int get_screen_width();
int get_screen_height();

void run_menu(ALLEGRO_EVENT_QUEUE*);

int main() {

    // load all this shit, because if you load it later weird stuff starts happening for some reason
    load_options();
    load_objects();

    // initialize display
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *events = NULL;
    ALLEGRO_TIMER *timer = NULL;

    if (!al_init()) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Allegro failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if (!al_install_mouse()) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Allegro mouse failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if (!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Allegro keyboard failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if (!al_init_image_addon()) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Allegro image addon failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Allegro ttf font addon failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    timer = al_create_timer(1.0/get_FPS());
    if (!timer) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Allegro timer failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    display = al_create_display(get_screen_width(), get_screen_height());
    if (!display) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Display failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        return -1;
    }

    events = al_create_event_queue();
    if (!events) {
        al_show_native_message_box(display, "Error", "", "Fatal Error: Allegro event queue failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_register_event_source(events, al_get_display_event_source(display));
    al_register_event_source(events, al_get_timer_event_source(timer));
    al_register_event_source(events, al_get_mouse_event_source());
    al_register_event_source(events, al_get_keyboard_event_source());

    // load images and stuff
    load_assets();
    al_hide_mouse_cursor(display);

    al_start_timer(timer);

    run_menu(events);

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(events);
    return 0;
}
*/
