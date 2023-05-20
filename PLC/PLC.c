#include "PLC.h"

typedef struct PLC_Insts
{
    void *d;
    struct PLC_Insts *n;

} PLC_Insts;

typedef struct PLC_Callbacks
{
    PLC_Callback f;
    PLC_Insts *l;
    struct PLC_Callbacks *n;

} PLC_Callbacks;

typedef struct PLC_PriGrps
{
    unsigned pri;
    PLC_Callbacks *l;
    struct PLC_PriGrps *n; 

} PLC_PriGrps;

inline void PLC_init(PLC_PriGrps **pL) { *pL = NULL; }
inline int PLC_empty(const PLC_PriGrps *l) { return l == NULL; }

static int PLC_cmpPriGrps(PLC_PriGrps *pg0, PLC_PriGrps *pg1)
{
    return pg0->pri-pg1->pri;
}

static int PLC_cmpCallbacks(PLC_Callbacks *cb0, PLC_Callbacks *cb1)
{
    return cb0->f-cb1->f;
}

static int PLC_cmpInsts(PLC_Insts *inst0, PLC_Insts *inst1)
{
    return inst0->d-inst1->d;
}

int PLC_add(PLC_PriGrps **pL, unsigned pri, PLC_Callback f, void *d)
{
    if (pL == NULL) return -1;

    PLC_PriGrps elemPG = {.pri=pri}, **memOrPosPG;
    int notFound;

    SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
        PLC_PriGrps,
        *pL,
        &elemPG,
        PLC_cmpPriGrps,
        n,
        notFound,
        memOrPosPG
    );

    if (notFound)
    {
        PLC_Insts *pInts = PLC_MALLOC(sizeof(PLC_Insts));
        if (pInts == NULL) return -1;

        PLC_Callbacks *pCB = PLC_MALLOC(sizeof(PLC_Callbacks));
        if (pCB == NULL) return -1;

        PLC_PriGrps *pPG = PLC_MALLOC(sizeof(PLC_PriGrps));
        if (pPG == NULL) return -1;

        *pInts = (PLC_Insts){d, NULL};
        *pCB = (PLC_Callbacks){f, pInts, NULL};
        *pPG = (PLC_PriGrps){pri, pCB, *memOrPosPG};
        *memOrPosPG = pPG;
    }
    else
    {
        PLC_Callbacks elemCB = {.f=f}, **memOrPosCB;

        SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
            PLC_Callbacks,
            (*memOrPosPG)->l,
            &elemCB,
            PLC_cmpCallbacks,
            n,
            notFound,
            memOrPosCB
        );

        if (notFound)
        {
            PLC_Insts *pInts = PLC_MALLOC(sizeof(PLC_Insts));
            if (pInts == NULL) return -1;

            PLC_Callbacks *pCB = PLC_MALLOC(sizeof(PLC_Callbacks));
            if (pCB == NULL) return -1;

            *pInts = (PLC_Insts){d, NULL};
            *pCB = (PLC_Callbacks){f, pInts, *memOrPosCB};
            *memOrPosCB = pCB;
        }
        else
        {
            PLC_Insts elemInst = {.d=d}, **memOrPosInst;

            SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
                PLC_Insts,
                (*memOrPosCB)->l,
                &elemInst,
                PLC_cmpInsts,
                n,
                notFound,
                memOrPosInst
            );

            if (notFound)
            {
                PLC_Insts *pInts = PLC_MALLOC(sizeof(PLC_Insts));
                if (pInts == NULL) return -1;

                *pInts = (PLC_Insts){d, *memOrPosInst};
                *memOrPosInst = pInts;
            }
            else return 1;
        }
    }

    return 0;
}

int PLC_remove(PLC_PriGrps **pL, unsigned pri, PLC_Callback f, void *d)
{
    if (pL == NULL) return -1;

    if (*pL == NULL) return 1;

    PLC_PriGrps elemPG = {.pri=pri}, **memOrPosPG;
    int notFound;

    SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
        PLC_PriGrps,
        *pL,
        &elemPG,
        PLC_cmpPriGrps,
        n,
        notFound,
        memOrPosPG
    );

    if (notFound) return 1;
    else
    {
        PLC_Callbacks elemCB = {.f=f}, **memOrPosCB;

        SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
            PLC_Callbacks,
            (*memOrPosPG)->l,
            &elemCB,
            PLC_cmpCallbacks,
            n,
            notFound,
            memOrPosCB
        );

        if (notFound) return 1;
        else
        {

            PLC_Insts elemInst = {.d=d}, **memOrPosInst;

            SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
                PLC_Insts,
                (*memOrPosCB)->l,
                &elemInst,
                PLC_cmpInsts,
                n,
                notFound,
                memOrPosInst
            );

            if (notFound) return 1;
            else
            {
                if (
                    (*memOrPosCB)->l == *memOrPosInst &&
                    (*memOrPosInst)->n == NULL
                )
                {
                    PLC_FREE(*memOrPosInst);
                    *memOrPosInst = NULL;

                    if (
                        (*memOrPosPG)->l == *memOrPosCB &&
                        (*memOrPosCB)->n == NULL
                    )
                    {
                        PLC_FREE(*memOrPosCB);
                        *memOrPosCB = NULL;

                        if (
                            *pL == *memOrPosPG &&
                            (*memOrPosPG)->n == NULL
                        )
                        {
                            PLC_FREE(*memOrPosPG);
                            *memOrPosPG = NULL;
                        }
                        else
                        {
                            PLC_PriGrps *aux = *memOrPosPG;
                            *memOrPosPG = (*memOrPosPG)->n;
                            aux->n = NULL;
                            PLC_FREE(aux);
                        }
                    }
                    else
                    {
                        PLC_Callbacks *aux = *memOrPosCB;
                        *memOrPosCB = (*memOrPosCB)->n;
                        aux->n = NULL;
                        PLC_FREE(aux);
                    }
                }
                else
                {
                    PLC_Insts *aux = *memOrPosInst;
                    *memOrPosInst = (*memOrPosInst)->n;
                    aux->n = NULL;
                    PLC_FREE(aux);
                }
            }
        }
    }
}

void PLC_update(PLC_PriGrps **pL)
{
    PLC_PriGrps *memPG;

    SGLIB_SORTED_LIST_MAP_ON_ELEMENTS(
        PLC_PriGrps,
        *pL,
        memPG,
        n,
        {
            PLC_Callbacks *memCB;

            SGLIB_SORTED_LIST_MAP_ON_ELEMENTS(
                PLC_Callbacks,
                memPG->l,
                memCB,
                n,
                {
                    PLC_Insts *memInst;

                    SGLIB_SORTED_LIST_MAP_ON_ELEMENTS(
                        PLC_Insts,
                        memCB->l,
                        memInst,
                        n,
                        {
                            memCB->f(pL, memInst->d);
                        }
                    );
                }
            );
        }
    );
}
