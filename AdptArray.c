#include <malloc.h>
#include <string.h>
#include "AdptArray.h"

typedef struct AdptArray_ {
    int ArrSize;
    PElement *pElemArr;
    COPY_FUNC copy_func;
    DEL_FUNC del_func;
    PRINT_FUNC print_func;
} AdptArray;


PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func) {
    PAdptArray pArr = (PAdptArray) malloc(sizeof(AdptArray));
    // if failed initialize
    if (pArr == NULL)
        return NULL;
    // up to preference, if 0 its granted extend on first set
    pArr->ArrSize = 0;
    pArr->pElemArr = NULL;
    pArr->copy_func = copy_func;
    pArr->del_func = del_func;
    pArr->print_func = print_func;
    return pArr;
}

void DeleteAdptArray(PAdptArray pArr) {
    int i;
    if (pArr == NULL)
        return;
    for (i = 0; i < pArr->ArrSize; ++i) {
        // dont try delete null
        if ((pArr->pElemArr)[i] == NULL) continue;
        pArr->del_func((pArr->pElemArr)[i]);
    }
    // free the array itself
    free(pArr->pElemArr);
    free(pArr);
}

Result SetAdptArrayAt(PAdptArray pArr, int index, PElement element) {
    PElement *newpElemArr;
    if (pArr == NULL)
        return FAIL;

    // if need to resize
    if (index >= pArr->ArrSize) {
        //allocate memory
        if ((newpElemArr = (PElement *) calloc((index + 1), sizeof(PElement))) == NULL)
            //if failed dont continue
            return FAIL;
        //copy memory
        memcpy(newpElemArr, pArr->pElemArr, (pArr->ArrSize) * sizeof(PElement));
        //free the pre array
        free(pArr->pElemArr);
        //change the pointer to new array
        pArr->pElemArr = newpElemArr;
    }

    // Delete Previous Elem and free it
    if ((pArr->pElemArr)[index] != NULL)
        pArr->del_func((pArr->pElemArr)[index]);
    (pArr->pElemArr)[index] = pArr->copy_func(element);

    // Update Array Size
    pArr->ArrSize = (index >= pArr->ArrSize) ? (index + 1) : pArr->ArrSize;
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray pArr, int index) {
    if (pArr == NULL)
        return NULL;
    //check if index is in array size
    if (index >= pArr->ArrSize)
        return NULL;
    //check if not null so wont try to copy null
    if (pArr->pElemArr[index] == NULL)
        return NULL;
    //return COPY
    return pArr->copy_func(pArr->pElemArr[index]);
}

int GetAdptArraySize(PAdptArray pArr) {
    // if not initialize so return -1
    if (pArr == NULL)
        return -1;

    return pArr->ArrSize;
}

void PrintDB(PAdptArray pArr) {
    int i;
    if (pArr == NULL)
        return;

    for (i = 0; i < pArr->ArrSize; ++i) {
        // dont try print null
        if ((pArr->pElemArr)[i] == NULL) continue;
        pArr->print_func((pArr->pElemArr)[i]);
    }
}

