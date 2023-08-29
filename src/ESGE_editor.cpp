#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_audio.h"
#include <stdio.h>
#include "ESGE_objDraw.h"
#include "ESGE_error.h"


#define ESGE_CMD_LEN 128
#define ESGE_FIELD_VALUE_LEN 32


static bool run = true;


static void
CmdAdd(ESGE_Scene *scene, const char *delim, char **saveptr)
{
	char *typeName;
	ESGE_ObjScene *obj;

	if (!(typeName = SDL_strtokr(NULL, delim, saveptr)))
	{
		puts("Missing field name");
	}
	else if (!(obj = scene->AddObj(typeName)))
	{
		puts(SDL_GetError());
		SDL_ClearError();
	}
	else
	{
		char *fieldName, *fieldValue;

		while ((fieldName = SDL_strtokr(NULL, delim, saveptr)))
		{
			const ESGE_Type *type;
			const ESGE_Field *field;

			if (!(fieldValue = SDL_strtokr(NULL, delim, saveptr)))
			{
				printf("Missing field \"%s\" value\n", fieldName);
				break;
			}

			if (!(type = ESGE_Type::Get(obj->typeID)))
				ESGE_Error("Type \"%s\" not found?", typeName);

			if (!(field = type->GetField(fieldName)))
			{
				printf("Field \"%s\" not found\n", fieldName);
				break;
			}
			else
			{
				field->SetValue(obj, fieldValue);
			}
		}
	}
}

static void
CmdDelete(ESGE_Scene *scene, const char *delim, char **saveptr)
{
	char *instName;

	if (!(instName = SDL_strtokr(NULL, delim, saveptr)))
	{
		puts("Missing instance name");
	}
	else scene->DelObj(instName);
}

static void
CmdRename(ESGE_Scene *scene, const char *delim, char **saveptr)
{
	char *instName, *newInstName;

	if (!(instName = SDL_strtokr(NULL, delim, saveptr)))
	{
		puts("Missing old instance name");
	}
	else if (!(newInstName = SDL_strtokr(NULL, delim, saveptr)))
	{
		puts("Missing new instance name");
	}
	else if (scene->RenameObj(instName, newInstName))
	{
		puts(SDL_GetError());
		SDL_ClearError();
	}
}

static void
CmdPrint(ESGE_Scene *scene, const char *delim, char **saveptr)
{
	char *instName;
	ESGE_ObjScene *obj;

	if (!(instName = SDL_strtokr(NULL, delim, saveptr)))
	{
		puts("Missing instance name");
	}
	else if (!(obj = scene->GetObj(instName)))
	{
		puts(SDL_GetError());
		SDL_ClearError();
	}
	else
	{
		const ESGE_Type *type;

		if (!(type = ESGE_Type::Get(obj->typeID)))
			ESGE_Error("Object \"%s\" without type?", instName);

		for (size_t i = 0; i < type->nFields; ++i)
		{
			char buf[ESGE_FIELD_VALUE_LEN];
				
			type->fields[i].GetValue(obj, buf, ESGE_FIELD_VALUE_LEN);
			printf("%s = %s\n", type->fields[i].name, buf);
		}
	}
}

static void
CmdEdit(ESGE_Scene *scene, const char *delim, char **saveptr)
{
	char *instName;
	ESGE_ObjScene *obj;

	if (!(instName = SDL_strtokr(NULL, delim, saveptr)))
	{
		puts("Missing instance name");
	}
	else if (!(obj = scene->GetObj(instName)))
	{
		puts(SDL_GetError());
		SDL_ClearError();
	}
	else
	{
		char *fieldName, *fieldValue;

		while ((fieldName = SDL_strtokr(NULL, delim, saveptr)))
		{
			const ESGE_Type *type;
			const ESGE_Field *field;

			if (!(fieldValue = SDL_strtokr(NULL, delim, saveptr)))
			{
				printf("Missing field \"%s\" value\n", fieldName);
				break;
			}

		if (!(type = ESGE_Type::Get(obj->typeID)))
			ESGE_Error("Object \"%s\" without type?", instName);

			if (!(field = type->GetField(fieldName)))
			{
				printf("Field \"%s\" not found\n", fieldName);
				break;
			}
			else
			{
				field->SetValue(obj, fieldValue);
			}
		}
	}
}

static void
CmdList(ESGE_Scene *scene)
{
	for (
		ESGE_ObjScene *obj = scene->objList;
		obj != NULL;
		obj = obj->next
	)
	{
		printf("\"%s\"\n", obj->instName);
	}
}

static void
CmdSave(ESGE_Scene *scene)
{
	if(scene->Save())
	{
		puts(SDL_GetError());
		SDL_ClearError();
	}
}

static void
CmdChange(ESGE_Scene **scene, const char *delim, char **saveptr)
{
	char *sceneFile;

	if (!(sceneFile = SDL_strtokr(NULL, delim, saveptr)))
	{
		puts("Missing scene file");
	}
	else if(ESGE_SceneMngr::SetActiveScene(sceneFile))
	{
		puts(SDL_GetError());
		SDL_ClearError();
	}
	else *scene = ESGE_SceneMngr::GetActiveScene();
}

static void
CmdGoing(ESGE_Scene *scene)
{
	printf("\"%s\"\n", scene->sceneFile);
}


static int
RunShell(SDL_UNUSED void *userdata)
{
	ESGE_Scene *scene;

	scene = ESGE_SceneMngr::GetActiveScene();

	for (;;)
	{
		static const char *const help = "help";
		static const char *const delim = " \n\r\t\b";
  	char line[ESGE_CMD_LEN], *cmd, *saveptr;

    fputs("> ", stdout);
    if (!fgets(line, ESGE_CMD_LEN, stdin))
    	ESGE_Error("Cannot read from %s", "stdin");

    if (!(cmd = SDL_strtokr(line, delim, &saveptr))) continue;

    if (cmd[1])
    {
	    if (!SDL_strcmp(cmd, "add"))
	  		CmdAdd(scene, delim, &saveptr);
	  	else if (!SDL_strcmp(cmd, "delete"))
	  		CmdDelete(scene, delim, &saveptr);
	  	else if (!SDL_strcmp(cmd, "rename"))
	  		CmdRename(scene, delim, &saveptr);
	  	else if (!SDL_strcmp(cmd, "print"))
	  		CmdPrint(scene, delim, &saveptr);
	  	else if (!SDL_strcmp(cmd, "edit"))
	  		CmdEdit(scene, delim, &saveptr);
	  	else if (!SDL_strcmp(cmd, "list"))
	  		CmdList(scene);
	  	else if (!SDL_strcmp(cmd, "save"))
	  		CmdSave(scene);
	  	else if (!SDL_strcmp(cmd, "change"))
	  		CmdChange(&scene, delim, &saveptr);
	  	else if (!SDL_strcmp(cmd, "going"))
	  		CmdGoing(scene);
	  	else if (!SDL_strcmp(cmd, "quit"))
	  		break;
	  	else puts(help);
		}
		else
		{
			bool stop = false;

			switch (cmd[0])
			{
			case 'a':
				CmdAdd(scene, delim, &saveptr);
				break;
			case 'd':
				CmdDelete(scene, delim, &saveptr);
				break;
			case 'r':
				CmdRename(scene, delim, &saveptr);
				break;
			case 'p':
				CmdPrint(scene, delim, &saveptr);
				break;
			case 'e':
				CmdEdit(scene, delim, &saveptr);
				break;
			case 'l':
				CmdList(scene);
				break;
			case 's':
				CmdSave(scene);
				break;
			case 'c':
				CmdChange(&scene, delim, &saveptr);
				break;
			case 'g':
				CmdGoing(scene);
				break;
			case 'q':
				stop = true;
				break;
			default:
				puts(help);
			}

			if (stop) break;
		}
	}
	run = false;

	return 0;
}

Uint32 ESGE_deltaTm = 16, ESGE_realDeltaTm;

#define ESGE_EDITOR_CAM_VEL (ESGE_deltaTm / 16)
#define ESGE_EDITOR_ZOOM 1.f + 0.01f * ((float)ESGE_deltaTm) / 16.f

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
	const char *title = "editor", *scene = "scene.bin";
	int w = 256, h = 144;
	SDL_Thread *shell;
	Uint32 ticks;

	if (
		SDL_Init(
			SDL_INIT_EVENTS |
			SDL_INIT_VIDEO |
			SDL_INIT_AUDIO |
			SDL_INIT_JOYSTICK
		)
	)
		ESGE_Error("Failed to initializa SDL: %s", SDL_GetError());

	ESGE_Display::Init(title, w, h);
	ESGE_InitAudio(0, 0);
	ESGE_SceneMngr::Init(0);

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
			case 'f':
				SDL_sscanf(argv[++i], "%" SDL_PRIu32, &ESGE_deltaTm);
				ESGE_deltaTm = 1000 / ESGE_deltaTm;
				break;
			default:
				puts("help");
				return 0;
			}
		}
		else
		{
			ESGE_SceneMngr::AddScene(argv[i]);
			scene = NULL;
		}
	}

	if (scene) ESGE_SceneMngr::AddScene(scene);

	ESGE_SceneMngr::Update();
	shell = SDL_CreateThread(RunShell, "shell", NULL);

	while (run)
	{
		ticks = SDL_GetTicks();

		SDL_PumpEvents();
		UpdateEditor();
		ESGE_SceneMngr::Update();
		ESGE_ObjDraw::Draw();
		ESGE_Display::Update();

		ESGE_realDeltaTm = SDL_GetTicks() - ticks;
		
  	if (ESGE_realDeltaTm < ESGE_deltaTm)
  		SDL_Delay(ESGE_deltaTm - ESGE_realDeltaTm);
	}

	SDL_WaitThread(shell, NULL);

	ESGE_SceneMngr::Quit();
	ESGE_QuitAudio();
	ESGE_Display::Quit();
	SDL_Quit();

	return 0;
}