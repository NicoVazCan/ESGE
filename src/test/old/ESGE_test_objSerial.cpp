#include <SDL2/SDL.h>
#include "ESGE_objSerial.h"

class ObjTest;

class ObjTest: public ESGE_ObjSerial
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
	Uint8 data[] = {
		0x00, 0x01,
		0x00, 0x02,
		'l','o','l','\0','\0','\0'
	};
	SDL_RWops *io;
	ObjTest *obj;

	obj = (ObjTest*)ESGE_Type::New("ObjTest");

	SDL_Log("%hhX, %hhX, %s", obj->pos.x, obj->pos.y, obj->name);

	io = SDL_RWFromMem(data, sizeof (data));

	obj->OnLoad(io);

	SDL_Log("%hhX, %hhX, %s", obj->pos.x, obj->pos.y, obj->name);

	obj->SetField("pos.x", "4");
	obj->SetField("pos.y", "5");
	obj->SetField("name", "hola");

	SDL_Log("%hhX, %hhX, %s", obj->pos.x, obj->pos.y, obj->name);

	SDL_RWseek(io, 0, RW_SEEK_SET);

	obj->OnSave(io);

	SDL_RWclose(io);

	SDL_Log("%hhX, %hhX, %s", data[1], data[3], data+4);

	delete obj;

	return 0;
}