#include <SDL2/SDL.h>
#include "ESGE_file.h"
#include "ESGE_scene.h"
#include <stdio.h>


class ObjTest: public ESGE_ObjScene
{
public:
	SDL_Point pos = {0,0};
	char name[6] = "";

	ObjTest(void)
	{
		SDL_Log("%s", SDL_FUNCTION);
	}
	virtual ~ObjTest(void) override
	{
		SDL_Log("%s", SDL_FUNCTION);
	}

	virtual const ESGE_Field *GetFields(size_t *nFields) const
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

int main(SDL_UNUSED int argc, SDL_UNUSED char const *argv[])
{
	ESGE_Scene *scene;
	ESGE_ObjScene *obj;
	char buf[16];
	const ESGE_Field *fields;
	size_t nFields;


	scene = ESGE_FileMngr<ESGE_Scene>::Watch("test.bin");

	obj = scene->AddObj("ObjTest");

	fields = obj->GetFields(&nFields);

	for (size_t i = 0; i < nFields; ++i)
	{
		obj->GetFieldValue(fields+i, buf, 16);
		printf("%s=%s\n", fields[i].name, buf);
	}

	scene->Save();

	ESGE_FileMngr<ESGE_Scene>::Leave(scene);
	
	//remove("test.bin");

	return 0;
}