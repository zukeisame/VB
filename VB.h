#ifndef _VB_
#define _VB_
// ================================================================================================
// DEFINITION =====================================================================================
// ================================================================================================
// short for "struct VariableBuffer"
typedef struct VariableBuffer VB;

/* 
 * The Followings are Declarations of Helper Functions, You Should Only Use Symbols in the Manuals
 */
VB* vbNewDetail(const char *const, const unsigned long, long);

int vbIsTypeDetail(const VB *const, const char *const);

void vbPushGeneric(VB *const, const char *const, const void *const);

void* vbPopGeneric(VB *const, const char *const);

void* vbGetGeneric(VB *const, const char *const, const long);

void* vbCopyArrayGeneric(const VB *const, const char *const);

void* vbToArrayGeneric(VB *const, const char *const);

void vbTypeCastDetail(VB *const, const char *const, const unsigned long);

long vbFindGeneric(const VB *const, const char *const, const void *const);
// ================================================================================================
// MANUALS ========================================================================================
// ================================================================================================
// create a new VB with specified type and initial size, ex: vbNew(int, 2), a negative vblue stands for default size
// remember to free it when you're done using vbfree(vb)
#define vbNew(type, sizeOfBuffer) vbNewDetail(#type, sizeof(type), sizeOfBuffer)

// check whether "type" match the element type of this VB
#define vbIsType(vb, type) vbIsTypeDetail(vb, #type)

// current VB is empty or not
int vbIsEmpty(const VB *const vb);

// get type of this VB in string
const char* vbGetType(const VB *const vb);

// get how many elements in this VB
long vbGetBufferSize(const VB *const vb);

// get the stored element size of this VB
unsigned long vbGetElementSize(const VB *const vb);

// empty this VB
void vbClear(VB *const vb);

// release all the spaces used by this VB, remember to call it every time you're done
void vbFree(VB *const vb);

// create a new VB exactly like the original one, remenber to free it after use
VB* vbCopy(const VB *const vb);

// return a new subVB by the indexes specified by the arguments, remenber to free it after use
VB* vbSubVB(const VB *const vb, const long startIndex, const long endIndex);

// push a new element at the end of VB, ex: vbPush(vb, int, 33)
#define vbPush(vb, type, newElement) vbPushGeneric(vb, #type, (type[]) {newElement})

// return the last element out of VB and delete it, ex: vbPop(vb, int)
#define vbPop(vb, type) (*(type*) vbPopGeneric(vb, #type))

// ex: vbGet(vb, int, 3), which is just like reference vb[3] in an array
#define vbGet(vb, type, index) (*((type*) vbGetGeneric(vb, #type, index)))

// retrun the internal array of VB, ex: vbGetArray(vb, int), modify it will modify original VB too
void* vbGetArrayGeneric(const VB *const vb, const char *const typeName);
#define vbGetArray(vb, type) ((type*) vbGetArrayGeneric(vb, #type))

// return the internal array as string
#define vbGetString(vb) vbGetArray(vb, char)

// return a new array same as the internal array of VB, modify it will not affect the original VB, remenber to free it after use
#define vbCopyArray(vb, type) ((type*) vbCopyArrayGeneric(vb, #type))

// return a new string same as the internal array of VB, modify it will not affect the original VB, remenber to free it after use
#define vbCopyString(vb) vbCopyArray(vb, char)

// release the VB but leave the internal array and return it, remenber to free it after use
#define vbToArray(vb, type) ((type*) vbToArrayGeneric(vb, #type))

// release the VB but leave the internal array and return it as string
#define vbToString(vb) vbToArray(vb, char)

// cast VB's internal type to a new type, types should be convertible, or it will fail
#define vbTypeCast(vb, type) vbTypeCastDetail(vb, #type, sizeof(type))

// return index of the element if VB contains it, negative otherwise, ex: vbFind(vb, int, 13)
#define vbFind(vb, type, element) vbFindGeneric(vb, #type, (type[]) {element})

// qsort VB using the function pointer (i.e. argument "compare"), function prototype: int compareFunction(const void*, const void*)
void vbSort(const VB *const vb, int compare(const void*, const void*));

// traverse each element in the VB, using function to process each element, function prototype: void function(void*)
void vbForEach(const VB *const vb, void function(void*));
// ================================================================================================
// ================================================================================================
// ================================================================================================
#endif