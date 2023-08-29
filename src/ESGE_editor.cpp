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

#define SHELL_HELP \
"List of editor commands:\n" \
"\n" \
"add OBJ_TYPE_NAME [FIELD_NAME FIELD_VALUE]...\n" \
"  Adds a new instance of object type OBJ_TYPE_NAME to active\n" \
"  scene and optionally edit the value of its fields. The\n" \
"  instance will be automatically named with the its type name\n"\
"  plus a numerical suffix.\n" \
"\n" \
"delete OBJ_INSTANCE_NAME\n" \
"  Deletes the instance named as OBJ_INSTANCE_NAME from the\n" \
"  active scene if exist.\n" \
"\n" \
"rename OBJ_INSTANCE_NAME NEW_OBJ_INSTANCE_NAME\n" \
"  Renames the instance named as OBJ_INSTANCE_NAME of active\n" \
"  scene with NEW_OBJ_INSTANCE_NAME if not already used.\n" \
"\n" \
"print OBJ_INSTANCE_NAME\n" \
"  Prints the name and value of all the fields of the instance\n" \
"  named as OBJ_INSTANCE_NAME of the active scene.\n" \
"\n" \
"edit OBJ_INSTANCE_NAME [FIELD_NAME FIELD_VALUE]...\n" \
"  Edits the value of the field FIELD_NAME belonging to instance\n" \
"  named as OBJ_INSTANCE_NAME with FIELD_VALUE.\n" \
"\n" \
"list\n" \
"  List all instance names of the active scene objects.\n" \
"\n" \
"save\n" \
"  Saves the active scene to the file with its name.\n" \
"\n" \
"change SCENE_FILE\n" \
"  Changes the active scene with the scene loaded from\n" \
"  SCENE_FILE.\n" \
"\n" \
"going\n" \
"  Shows the scene file name of the active scene.\n" \
"\n" \
"quit\n" \
"  Close the editor prompt, window, and scenes without saving.\n" \
"\n" \
"help\n" \
"  Prints this mensage.\n" \
"\n" \
"It is possible to type only the first letter of the above\n" \
"commands for brevity.\n"


static int
RunShell(SDL_UNUSED void *userdata)
{
	ESGE_Scene *scene;

	scene = ESGE_SceneMngr::GetActiveScene();

	for (;;)
	{
		static const char *const help = SHELL_HELP;
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

#define ESGE_EDITOR_CAM_VEL ((float) ESGE_deltaTm * 4 / 16)
#define ESGE_EDITOR_ZOOM 1.f + 0.01f * ((float)ESGE_deltaTm) / 16.f

void
UpdateEditor(void)
{
	static SDL_FPoint pos = {0.f, 0.f}, scale;
	const Uint8 *keys;

	SDL_RenderGetScale(ESGE_Display::renderer, &scale.x, &scale.y);
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_UP])
	{
		pos.y -= ESGE_EDITOR_CAM_VEL / scale.y;
		ESGE_Display::cam.y = pos.y;
	}
	if (keys[SDL_SCANCODE_DOWN])
	{
		pos.y += ESGE_EDITOR_CAM_VEL / scale.y;
		ESGE_Display::cam.y = pos.y;
	}
	if (keys[SDL_SCANCODE_LEFT])
	{
		pos.x -= ESGE_EDITOR_CAM_VEL / scale.x;
		ESGE_Display::cam.x = pos.x;
	}
	if (keys[SDL_SCANCODE_RIGHT])
	{
		pos.x += ESGE_EDITOR_CAM_VEL / scale.x;
		ESGE_Display::cam.x = pos.x;
	}

	if (keys[SDL_SCANCODE_KP_PLUS])
	{
		scale.x *= ESGE_EDITOR_ZOOM;
		scale.y *= ESGE_EDITOR_ZOOM;

		SDL_RenderSetScale(ESGE_Display::renderer, scale.x, scale.y);
	}
	if (keys[SDL_SCANCODE_KP_MINUS])
	{
		scale.x /= ESGE_EDITOR_ZOOM;
		scale.y /= ESGE_EDITOR_ZOOM;

		SDL_RenderSetScale(ESGE_Display::renderer, scale.x, scale.y);
	}

	if (keys[SDL_SCANCODE_SPACE])
	{
		Uint32 buttons;
		int x, y;

		buttons = SDL_GetRelativeMouseState(&x, &y);

		if (SDL_BUTTON(SDL_BUTTON_LEFT) & buttons)
		{
			pos.x -= ((float)x) / scale.x;
			pos.y -= ((float)y) / scale.y;

			ESGE_Display::cam.x = pos.x;
			ESGE_Display::cam.y = pos.y;
		}
	}
}

#define EDIT_HELP \
"USAGE: editor [OPTION]... [FILE]...\n" \
"Opens a window in which the scenes loaded from the FILEs will be\n" \
"displayed while the editor commands are read from the standard\n" \
"input.\n" \
"If no FILE specified, the scene is opened from the file named\n" \
"\"scene.bin\" in the current directory, if it does not exist an\n" \
"empty one is created.\n" \
"\n" \
"  -W Set window width in pixels, 256 by default\n" \
"  -H Set window width in pixels, 144 by default\n" \
"  -f Set game loop frame per secound, 60 by default\n" \
"  -h Print this mensage\n"

#ifdef __cplusplus
extern "C"
#endif
int
main(int argc, char *argv[])
{
	const char *title = "editor", *scene = "scene.bin";
	static const char *const help = EDIT_HELP;
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
				puts(help);
				return 0;
			}
			switch (argv[i][1])
			{
			case 'W':
				SDL_sscanf(argv[++i], "%d", &w);
				break;
			case 'H':
				SDL_sscanf(argv[++i], "%d", &h);
				break;
			case 'f':
				SDL_sscanf(argv[++i], "%" SDL_PRIu32, &ESGE_deltaTm);
				ESGE_deltaTm = 1000 / ESGE_deltaTm;
				break;
			case 'h':
			default:
				puts(help);
				ESGE_SceneMngr::Quit();
				ESGE_QuitAudio();
				ESGE_Display::Quit();
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