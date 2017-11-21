#include "SDL.h"
#include <iostream>
#include "box.h"
#include "position.h"
#include "button.h"
#include <vector>
#include <Windows.h>
#include <thread>
#include <fstream>
using namespace std;

//time and fps
const Uint32 fps = 30;
const Uint32 minimumFrameTime = 1000 / fps;

//dimensions
const int screenWidth = 700;
const int screenHeight = 500;

//piano shapes ect...
box pianoBox;
box piano;

//other
const float soundDuration = 1000;
char pressedKey = 'x';

//buttons & notes
std::vector<button> notes;
button quitButton;
button playButton;

// sound
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play
#define MUS_PATH1 "1.wav"
#define MUS_PATH2 "2.wav"
#define MUS_PATH3 "3.wav"
#define MUS_PATH4 "4.wav"
#define MUS_PATH5 "5.wav"
#define MUS_PATH6 "6.wav"
#define MUS_PATH7 "7.wav"
#define MUS_PATH8 "8.wav"
#define MUS_PATH9 "9.wav"
#define MUS_PATH0 "0.wav"
//sourced code starts here
						 // audio callback function
						 // here you have to copy the data of your audio buffer into the
						 // requesting audio buffer (stream)
						 // you should only copy as much as the requested length (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len) {

	if (audio_len == 0)
		return;

	len = (len > audio_len ? audio_len : len);
	//SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

	audio_pos += len;
	audio_len -= len;
}



void playSound(int pathNumber) {

	// local variables
	static Uint32 wav_length; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music


								   /* Load the WAV */
								   // the specs, length and buffer of our wav are filled
	switch (pathNumber)
	{
	case 1: SDL_LoadWAV(MUS_PATH1, &wav_spec, &wav_buffer, &wav_length); break;
	case 2: SDL_LoadWAV(MUS_PATH2, &wav_spec, &wav_buffer, &wav_length); break;
	case 3: SDL_LoadWAV(MUS_PATH3, &wav_spec, &wav_buffer, &wav_length); break;
	case 4: SDL_LoadWAV(MUS_PATH4, &wav_spec, &wav_buffer, &wav_length); break;
	case 5: SDL_LoadWAV(MUS_PATH5, &wav_spec, &wav_buffer, &wav_length); break;
	case 6: SDL_LoadWAV(MUS_PATH6, &wav_spec, &wav_buffer, &wav_length); break;
	case 7: SDL_LoadWAV(MUS_PATH7, &wav_spec, &wav_buffer, &wav_length); break;
	case 8: SDL_LoadWAV(MUS_PATH8, &wav_spec, &wav_buffer, &wav_length); break;
	case 9: SDL_LoadWAV(MUS_PATH9, &wav_spec, &wav_buffer, &wav_length); break;
	case 0: SDL_LoadWAV(MUS_PATH0, &wav_spec, &wav_buffer, &wav_length); break;
	default:
		break;
	}

	// set the callback function
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length

							/* Open the audio device */
	if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	/* Start playing */
	SDL_PauseAudio(0);

	// wait until we're don't playing
	/*while (audio_len > 0) {
		SDL_Delay(100);
	}*/
	SDL_Delay(250);

	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
}
//sourced code finish here
void readFile() {
	fstream in_file;
	in_file.open("song.txt");

	int note;

	while (in_file >> note) {
		playSound(note);
	}
	in_file.close();
}

//called when program ends
void close(SDL_Renderer* renderer, SDL_Window* window) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//check if button is pressed
void eventCheck(bool& quit) {
	pressedKey = 'x';
	SDL_Event event;
	while (!quit && SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE: quit = true; break;
			case SDLK_1: pressedKey = '1'; break;
			case SDLK_2: pressedKey = '2'; break;
			case SDLK_3: pressedKey = '3'; break;
			case SDLK_4: pressedKey = '4'; break;
			case SDLK_5: pressedKey = '5'; break;
			case SDLK_6: pressedKey = '6'; break;
			case SDLK_7: pressedKey = '7'; break;
			case SDLK_8: pressedKey = '8'; break;
			case SDLK_9: pressedKey = '9'; break;
			case SDLK_0: pressedKey = '0'; break;
			default	   :
				break;
			}
			break;
		default:
			break;
		}
	}
}

void initGui() {
	SDL_Color pbColor; pbColor.a = 255; pbColor.b = 1; pbColor.g = 1; pbColor.r = 1;
	pianoBox = box(pbColor,810,230,position(87,135));
	
	pbColor.a = 255; pbColor.b = 205; pbColor.g = 205; pbColor.r = 205;
	piano = box(pbColor, 860, 320, position(70, 120));

	SDL_Color clicked;	clicked.a = 255;	clicked.b = 50;	clicked.r = 255;	clicked.g = 255;
	SDL_Color hover;	hover.a = 255;	hover.g = 255;
	SDL_Color df;	df.r = 0;	df.a = 40;	df.g = 40;	df.b = 40;
	quitButton = button(clicked, hover, df, 80, 40, position(300, 400));
	playButton = button(clicked, hover, df, 80, 40, position(570, 400));

}

void initNotes() {
	float y = 150;

	for (int x = 100; x < 900; x += 80) {
		SDL_Color clickedColor; clickedColor.a = 255; clickedColor.b = 255; clickedColor.g = 255; clickedColor.r = 255;
		SDL_Color hoverColor;  hoverColor.a = 255; hoverColor.b = 100; hoverColor.g = 100; hoverColor.r = 100;
		SDL_Color defaultColor; defaultColor.a = 255; defaultColor.b = 200; defaultColor.g = 200; defaultColor.r = 200;

		position location = position(x, y);
		button newNote = button(clickedColor, hoverColor, defaultColor, 70, 200, location);

		notes.push_back(newNote);
	}
}

void drawGUI(SDL_Renderer* renderer) {
	piano.render(renderer);
	pianoBox.render(renderer);
	quitButton.draw(renderer);
	playButton.draw(renderer);
	
	for (size_t x = 0; x < notes.size(); x++ ) {
		notes[x].draw(renderer);
	}
}

void caculations(SDL_Renderer* renderer, SDL_Window* window) {
	if (quitButton.isClicked(renderer)) {
		close(renderer, window);
	}
	if (playButton.isClicked(renderer)) {
		readFile();
	}
}


void sounds(SDL_Renderer* renderer) {
	for (size_t x = 0; x < notes.size(); x++) {
		if (notes[x].isClicked(renderer)) {
			playSound(x);
		}
	}

	if (pressedKey != 'x') {
		playSound((int)pressedKey - 48);
	}	
}

void draw(SDL_Renderer* thingToDrawTO) {
	drawGUI(thingToDrawTO);
}

int main(int argc, char* args[]) {

	//init
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Piano", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 700, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	initGui();
	initNotes();

	//used to exit game loop
	bool quit = false;

	//time stuff
	Uint32 frameTime;
	Uint32 lastFrameTime = 0;
	Uint32 deltaTime = 0;

	//game loop
	while (!quit) {
		//fixed fps time stuff

		frameTime = SDL_GetTicks();
		deltaTime = frameTime - lastFrameTime;
		lastFrameTime = frameTime;

		eventCheck				(quit);						//check for exit
		SDL_RenderClear			(renderer);					//clear screen
		caculations				(renderer, window);				//caculations
		draw					(renderer);					//prep drawing 
		sounds					(renderer);						//sounds 
		SDL_SetRenderDrawColor	(renderer, 255, 255, 255, 255);
		SDL_RenderPresent		(renderer);					//render screen

		if ((SDL_GetTicks() - frameTime) < minimumFrameTime)
			SDL_Delay(minimumFrameTime - (SDL_GetTicks() - frameTime));
	}

	close(renderer,window);

	return 0;
}