#include <SDL2/SDL.h>
#include "ESGE_file.h"
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include <stdio.h>

#include "ESGE_objDraw.h"


class ObjTest: public ESGE_ObjScene, public ESGE_ObjDraw
{
public:
	SDL_Point pos = {0,0};
	char name[6] = {'\0','\0','\0','\0','\0','\0'};

	ObjTest(void)
	{
		layer = 1;
		EnableDraw();
		puts(SDL_FUNCTION);
	}
	virtual ~ObjTest(void) override
	{
		DisableDraw();
		puts(SDL_FUNCTION);
	}

	virtual const ESGE_Field *GetFields(size_t *nFields) override
	{
		static ESGE_Field fields[] = {
			{
				ESGE_Field::S16,
				"pos.x",
				NULL,
				0
			},
			{
				ESGE_Field::S16,
				"pos.y",
				NULL,
				0
			},
			{
				ESGE_Field::STR,
				"name",
				NULL,
				6
			}
		};

		fields[0].data = &pos.x;
		fields[1].data = &pos.y;
		fields[2].data = name;
		*nFields = SDL_arraysize(fields);

		return fields;
	}

	virtual void OnDraw(void) override
	{
		SDL_Rect rect = {pos.x, pos.y, 160, 160};
		ESGE_Display::WorldDrawRect(rect, 255,0,0,255);
	}
};

ESGE_ADD_TYPE(ObjTest)




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
    cmd = SDL_strtokr(line, delim, &saveptr);

    if (!SDL_strcmp(cmd, "add"))
  	{
  		char *typeName;

  		if (!(typeName = SDL_strtokr(NULL, delim, &saveptr)))
  		{
  			puts(help);
  		}
  		else if (!scene->AddObj(typeName))
  		{
  			puts(SDL_GetError());
  			SDL_ClearError();
  		}
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


#define DELTA_TIME 33

int
main(int argc, char const *argv[])
{
	if (argc == 2)
	{
		SDL_Thread *shell;
		Uint32 ticks, delta;

		SDL_Init(SDL_INIT_VIDEO);
		ESGE_Display::Init();
		if (!(scene = ESGE_FileMngr<ESGE_Scene>::Watch(argv[1])))
		{
			puts(SDL_GetError());
			SDL_ClearError();
			return -1;
		}
		shell = SDL_CreateThread(RunShell, "shell", NULL);
  	ticks = SDL_GetTicks();

		while (run)
		{
			ESGE_ObjDraw::Draw();
			ESGE_Display::Update();

			ticks += delta = SDL_GetTicks() - ticks;
    	if (delta < DELTA_TIME) SDL_Delay(DELTA_TIME - delta);
		}

		SDL_WaitThread(shell, NULL);
		ESGE_FileMngr<ESGE_Scene>::Leave(scene);
		ESGE_Display::Quit();
		SDL_Quit();
	}
	else
	{
		puts("help");
	}

	return 0;
}