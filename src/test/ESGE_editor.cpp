#include <SDL2/SDL.h>
#include "ESGE_file.h"
#include "ESGE_scene.h"
#include <stdio.h>

//#include "ESGE_objEditor.h"


class ObjTest: public ESGE_ObjScene
{
public:
	SDL_Point pos = {0,0};
	char name[6] = "";

	ObjTest(void)
	{
		puts(SDL_FUNCTION);
	}
	virtual ~ObjTest(void) override
	{
		puts(SDL_FUNCTION);
	}

	virtual const ESGE_Field *GetFields(size_t *nFields) const override
	{
		static const ESGE_Field fields[] = {
			{
				ESGE_Field::S16,
				"pos.x",
				OFFSETOF(ObjTest, pos) + OFFSETOF(SDL_Point, x),
				0
			},
			{
				ESGE_Field::S16,
				"pos.y",
				OFFSETOF(ObjTest, pos) + OFFSETOF(SDL_Point, y),
				0
			},
			{
				ESGE_Field::STR,
				"name",
				OFFSETOF(ObjTest, name),
				6
			}
		};

		*nFields = SDL_arraysize(fields);
		return fields;
	}
};

ESGE_ADD_TYPE(ObjTest)

#define ESGE_CMD_LEN 128
#define ESGE_FIELD_VALUE_LEN 32




int main(int argc, char const *argv[])
{
	static const char *const help = "help";


	//SDL_Init(SDL_INIT_VIDEO);

	if (argc == 2)
	{
		ESGE_Scene *scene;
		//ESGE_ObjEditorCam cam;

		if (!(scene = ESGE_FileMngr<ESGE_Scene>::Watch(argv[1])))
		{
			printf("Error: %s\n", SDL_GetError());
			SDL_ClearError();
			return -1;
		}

		for (;;)
		{
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
    			printf("Error: %s\n", SDL_GetError());
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
    			printf("Error: %s\n", SDL_GetError());
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
    			printf("Error: %s\n", SDL_GetError());
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
    					
    				if (
    					obj->GetFieldValue(fields+i, buf, ESGE_FIELD_VALUE_LEN)
    				)
    				{
			    		{
			    			printf("Error: %s\n", SDL_GetError());
			    			SDL_ClearError();
			    		}
    				}
    				else
    				{
    					printf("%s = %s\n", fields[i].name, buf);
    				}
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
    			printf("Error: %s\n", SDL_GetError());
    			SDL_ClearError();
    		}
    		else
    		{
    			while ((fieldName = SDL_strtokr(NULL, delim, &saveptr)))
    			{
    				if (!(fieldValue = SDL_strtokr(NULL, delim, &saveptr)))
    				{
    					printf("Missing field %s value\n", fieldName);
    					break;
    				}
    				
    				if (obj->SetFieldValue(fieldName, fieldValue))
    				{
    					printf("Error: %s\n", SDL_GetError());
    					SDL_ClearError();
    					break;
    				}
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
    		scene->Save();
    	}
    	else if (!SDL_strcmp(cmd, "exit"))
    	{
    		break;
    	}
    	else puts(help);
		}

		ESGE_FileMngr<ESGE_Scene>::Leave(scene);
	}
	else
	{
		puts(help);
	}

	//SDL_Quit();

	return 0;
}