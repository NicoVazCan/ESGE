#include <SDL2/SDL.h>
#include "ESGE_file.h"
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include <stdio.h>
#include "ESGE_objDraw.h"


#define ESGE_CMD_LEN 128
#define ESGE_FIELD_VALUE_LEN 32


static bool run = true; 
static ESGE_Scene *scene;


static int
RunShell(SDL_UNUSED void *userdata)
{
	for (;;)
	{
		static const char *const help = "help";
		static const char *const delim = " \n\r\t\b";
  	char line[ESGE_CMD_LEN], *cmd, *saveptr;

    fputs("> ", stdout);
    fgets(line, ESGE_CMD_LEN, stdin);
    if (!(cmd = SDL_strtokr(line, delim, &saveptr))) continue;

    if (!SDL_strcmp(cmd, "add"))
  	{
  		char *typeName;
  		ESGE_ObjScene *obj;

  		if (!(typeName = SDL_strtokr(NULL, delim, &saveptr)))
  		{
  			puts(help);
  		}
  		else if (!(obj = scene->AddObj(typeName)))
  		{
  			puts(SDL_GetError());
  			SDL_ClearError();
  		}
  		obj->OnEditorInit();
  	}
  	else if (!SDL_strcmp(cmd, "delete"))
  	{
  		char *instName;

  		if (!(instName = SDL_strtokr(NULL, delim, &saveptr)))
  		{
  			puts(help);
  		}
  		else scene->DelObj(instName);
  	}
  	else if (!SDL_strcmp(cmd, "rename"))
  	{
  		char *instName, *newInstName;

  		if (!(instName = SDL_strtokr(NULL, delim, &saveptr)))
  		{
  			puts(help);
  		}
  		else if (!(newInstName = SDL_strtokr(NULL, delim, &saveptr)))
  		{
  			puts(help);
  		}
  		else if (scene->RenameObj(instName, newInstName))
  		{
  			puts(SDL_GetError());
  			SDL_ClearError();
  		}
  	}
  	else if (!SDL_strcmp(cmd, "print"))
  	{
  		char *instName;
  		ESGE_ObjScene *obj;

  		if (!(instName = SDL_strtokr(NULL, delim, &saveptr)))
  		{
  			puts(help);
  		}
  		else if (!(obj = scene->GetObj(instName)))
  		{
  			puts(SDL_GetError());
  			SDL_ClearError();
  		}
  		else
  		{
  			const ESGE_Field *fields;
  			size_t nFields;

  			fields = obj->GetFields(&nFields);

  			for (size_t i = 0; i < nFields; ++i)
  			{
  				char buf[ESGE_FIELD_VALUE_LEN];
  					
					fields[i].GetValue(buf, ESGE_FIELD_VALUE_LEN);
					printf("%s = %s\n", fields[i].name, buf);
  			}
  		}
  	}
  	else if (!SDL_strcmp(cmd, "edit"))
  	{
  		char *instName, *fieldName, *fieldValue;
  		ESGE_ObjScene *obj;

  		if (!(instName = SDL_strtokr(NULL, delim, &saveptr)))
  		{
  			puts(help);
  		}
  		else if (!(obj = scene->GetObj(instName)))
  		{
  			puts(SDL_GetError());
  			SDL_ClearError();
  		}
  		else
  		{
  			while ((fieldName = SDL_strtokr(NULL, delim, &saveptr)))
  			{
  				const ESGE_Field *fields;
  				size_t nFields, i = 0;

  				if (!(fieldValue = SDL_strtokr(NULL, delim, &saveptr)))
  				{
  					printf("Missing field %s value\n", fieldName);
  					break;
  				}
  				fields = obj->GetFields(&nFields);

  				for (; i < nFields; ++i)
  				{
  					if (!SDL_strcmp(fields[i].name, fieldName))
  					{
  						fields[i].SetValue(fieldValue);
  						break;
  					}
  				}
  				if (i == nFields)	printf("Field %s not found\n", fieldName);
  			}
  		}
  	}
  	else if (!SDL_strcmp(cmd, "list"))
  	{
  		for (
  			ESGE_ObjScene *obj = scene->objList;
  			obj != NULL;
  			obj = obj->next
  		)
  		{
  			puts(obj->instName);
  		}
  	}
  	else if (!SDL_strcmp(cmd, "save"))
  	{
  		if(scene->Save())
  		{
  			puts(SDL_GetError());
				SDL_ClearError();
			}
  	}
  	else if (!SDL_strcmp(cmd, "exit"))
  	{
  		break;
  	}
  	else puts(help);
	}
	run = false;

	return 0;
}


#define ESGE_EDITOR_CAM_VEL 1
#define ESGE_EDITOR_ZOOM 1.01f

void
UpdateEditor(void)
{
	const Uint8 *keys;

	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_UP])
		ESGE_Display::cam.y -= ESGE_EDITOR_CAM_VEL;
	if (keys[SDL_SCANCODE_DOWN])
		ESGE_Display::cam.y += ESGE_EDITOR_CAM_VEL;
	if (keys[SDL_SCANCODE_LEFT])
		ESGE_Display::cam.x -= ESGE_EDITOR_CAM_VEL;
	if (keys[SDL_SCANCODE_RIGHT])
		ESGE_Display::cam.x += ESGE_EDITOR_CAM_VEL;

	if (keys[SDL_SCANCODE_KP_PLUS])
	{
		float scaleX, scaleY;

		SDL_RenderGetScale(ESGE_Display::renderer, &scaleX, &scaleY);

		scaleX *= ESGE_EDITOR_ZOOM;
		scaleY *= ESGE_EDITOR_ZOOM;

		SDL_RenderSetScale(ESGE_Display::renderer, scaleX, scaleY);

	}
	if (keys[SDL_SCANCODE_KP_MINUS])
	{
		float scaleX, scaleY;

		SDL_RenderGetScale(ESGE_Display::renderer, &scaleX, &scaleY);

		scaleX /= ESGE_EDITOR_ZOOM;
		scaleY /= ESGE_EDITOR_ZOOM;

		SDL_RenderSetScale(ESGE_Display::renderer, scaleX, scaleY);
	}

	if (keys[SDL_SCANCODE_SPACE])
	{
		Uint32 buttons;
		int x, y;

		buttons = SDL_GetRelativeMouseState(&x, &y);

		if (SDL_BUTTON(SDL_BUTTON_LEFT) & buttons)
		{
			float scaleX, scaleY;

			SDL_RenderGetScale(ESGE_Display::renderer, &scaleX, &scaleY);
			ESGE_Display::cam.x -= SDL_lroundf(((float)x) / scaleX);
			ESGE_Display::cam.y -= SDL_lroundf(((float)y) / scaleY);
		}
	}
}

#ifdef __cplusplus
extern "C"
#endif
int
main(int argc, char *argv[])
{
	const char *title = "editor", *sceneFile = "scene.bin";
	int w = 256, h = 144;
	SDL_Thread *shell;
	Uint32 ticks, delta, maxDelta = 16;

	for (int i = 1; i < argc; ++i)
	{
		if ('-' == argv[i][0])
		{
			if (i == argc-1)
			{
				puts("help");
				return 0;
			}
			switch (argv[i][1])
			{
			case 'w':
				SDL_sscanf(argv[++i], "%d", &w);
				break;
			case 'h':
				SDL_sscanf(argv[++i], "%d", &h);
				break;
			default:
				puts("help");
				return 0;
			}
		}
		else sceneFile = argv[i];
	}

	SDL_Init(SDL_INIT_VIDEO);
	ESGE_Display::Init(title, w, h);
	if (!(scene = ESGE_FileMngr<ESGE_Scene>::Watch(sceneFile)))
	{
		puts(SDL_GetError());
		SDL_ClearError();
		return -1;
	}
	scene->EditorInit();
	shell = SDL_CreateThread(RunShell, "shell", NULL);
	ticks = SDL_GetTicks();

	while (run)
	{
		SDL_PumpEvents();
		UpdateEditor();
		ESGE_ObjDraw::Draw();
		ESGE_Display::Update();

		ticks += delta = SDL_GetTicks() - ticks;
  	if (delta < maxDelta) SDL_Delay(maxDelta - delta);
	}

	SDL_WaitThread(shell, NULL);
	scene->EditorQuit();
	ESGE_FileMngr<ESGE_Scene>::Leave(scene);
	ESGE_Display::Quit();
	SDL_Quit();

	return 0;
}