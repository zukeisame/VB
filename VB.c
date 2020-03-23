#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "VB.h"
// ================================================================================================
// STRUCT DECLARATION =============================================================================
// ================================================================================================
struct VariableBuffer {
	long currentIndex;
	long sizeOfBuffer;
	const char *typeName;
	unsigned long sizeOfElement;
	void *buffer;
};
// ================================================================================================
// ERROR HANDLING =================================================================================
// ================================================================================================
void vbStandardError(const char *msg) {
	perror(msg);
	exit(1);
}

void vbCustomError(const char *const message, ...) {
	va_list argv;
	fprintf(stderr, "[!] Fatal: ");
	va_start(argv, message);
	vfprintf(stderr, message, argv);
	va_end(argv);
	fprintf(stderr, "\n");
	exit(1);
}
// ================================================================================================
// BACKGROUNDS ====================================================================================
// ================================================================================================
VB* vbMallocVB(void) {
	VB *const vb = (VB*) malloc(sizeof(VB));
	if (vb == NULL) {
		vbStandardError("malloc()");
	}
	return vb;
}

void* vbMallocBuffer(const unsigned long size) {
	void *const buffer = malloc(size);
	if (buffer == NULL) {
		vbStandardError("malloc()");
	}
	return buffer;
}

void vbEnlargeBuffer(VB *const vb) {
	if (vb->sizeOfBuffer <= 0) {
		vb->sizeOfBuffer = 1;
	}

	void *const newBuffer = vbMallocBuffer(2 * vb->sizeOfBuffer * vb->sizeOfElement + 1);

	memcpy(newBuffer, vb->buffer, vb->currentIndex * vb->sizeOfElement + 1);
	free(vb->buffer);
	
	vb->sizeOfBuffer *= 2;
	vb->buffer = newBuffer;
}

void vbSetNullByte(const VB *const vb) {
	*((char*) (vb->buffer + vb->currentIndex * vb->sizeOfElement)) = '\0';
}
// ================================================================================================
// IMPLEMENTATIONS ================================================================================
// ================================================================================================
#define VB_DEFAULT_BUFFER_SIZE (16)
VB* vbNewDetail(const char *const typeName, const unsigned long sizeOfElement, long sizeOfBuffer) {
	if (typeName == NULL) {
		vbCustomError("vbNew(): type name is invalid");
	}
	if (sizeOfElement == 0) {
		vbCustomError("vbNew(): size of element is invalid");
	}
	if (sizeOfBuffer < 0) {
		sizeOfBuffer = VB_DEFAULT_BUFFER_SIZE;
	}

	VB *const vb = vbMallocVB();
	vb->currentIndex = 0;
	vb->sizeOfBuffer = sizeOfBuffer;
	vb->typeName = typeName;
	vb->sizeOfElement = sizeOfElement;
	vb->buffer = vbMallocBuffer(sizeOfBuffer * sizeOfElement + 1);
	vbSetNullByte(vb);

	return vb;
}
#undef VB_DEFAULT_BUFFER_SIZE

const char* vbGetType(const VB *const vb) {
	return vb->typeName;
}

int vbIsTypeDetail(const VB *const vb, const char *const type) {
	return strcmp(vb->typeName, type) == 0;
}

int vbIsEmpty(const VB *const vb) {
	return vb->currentIndex == 0;
}

long vbGetBufferSize(const VB *const vb) {
	return vb->currentIndex;
}

unsigned long vbGetElementSize(const VB *const vb) {
	return vb->sizeOfElement;
}

void vbClear(VB *const vb) {
	vb->currentIndex = 0;
	vbSetNullByte(vb);
}

void vbFree(VB *const vb) {
	free(vb->buffer);
	free(vb);
}

VB* vbCopy(const VB *const vb) {
	VB *const newVB = vbMallocVB();
	newVB->currentIndex = vb->currentIndex;
	newVB->sizeOfBuffer = vb->sizeOfBuffer;
	newVB->typeName = vb->typeName;
	newVB->sizeOfElement = vb->sizeOfElement;
	newVB->buffer = vbMallocBuffer(vb->sizeOfBuffer * vb->sizeOfElement + 1);
	memcpy(newVB->buffer, vb->buffer, vb->currentIndex * vb->sizeOfElement + 1);
	return newVB;
}

void vbPushGeneric(VB *const vb, const char *const typeName, const void *const newElement) {
	if (strcmp(vb->typeName, typeName) != 0) {
		vbCustomError("vbPush(): incompatible type \"%s\"", typeName);
	}
	if (vb->currentIndex >= vb->sizeOfBuffer) {
		vbEnlargeBuffer(vb);
	}

	memcpy(vb->buffer + vb->sizeOfElement * ((vb->currentIndex)++), newElement, vb->sizeOfElement);
	vbSetNullByte(vb);
}

void* vbPopGeneric(VB *const vb, const char *const typeName) {
	if (strcmp(vb->typeName, typeName) != 0) {
		vbCustomError("vbPop(): incompatible type \"%s\"", typeName);
	}

	if (vb->currentIndex == 0) {
		vbCustomError("vbPopBack(): VB underflow");
	} else {
		return (vb->buffer + (--(vb->currentIndex)) * vb->sizeOfElement);
	}
}

void* vbGetGeneric(VB *const vb, const char *const typeName, const long index) {
	if (strcmp(vb->typeName, typeName) != 0) {
		vbCustomError("vbGet(): incompatible type \"%s\"", typeName);
	}

	if (index < 0 || index >= vb->sizeOfBuffer) {
		vbCustomError("vbGetElement(): index %d out of bound", index);
	} else {
		return (vb->buffer + vb->sizeOfElement * index);
	}
}

void* vbGetArrayGeneric(const VB *const vb, const char *const typeName) {
	if (strcmp(vb->typeName, typeName) != 0) {
		vbCustomError("vbGetArray(): incompatible type \"%s\"", typeName);
	} else {
		return vb->buffer;
	}
}

void* vbCopyArrayGeneric(const VB *const vb, const char *const typeName) {
	if (strcmp(vb->typeName, typeName) != 0) {
		vbCustomError("vbCopyArray(): incompatible type \"%s\"", typeName);
	} else {
		void *const ptr = vbMallocBuffer(vb->currentIndex * vb->sizeOfElement + 1);
		memcpy(ptr, vb->buffer, vb->currentIndex * vb->sizeOfElement + 1);
		return ptr;
	}
}

void* vbToArrayGeneric(VB *const vb, const char *const typeName) {
	if (strcmp(vb->typeName, typeName) != 0) {
		vbCustomError("vbToArray(): incompatible type \"%s\"", typeName);
	} else {
		void *const ptr = vb->buffer;
		free(vb);
		return ptr;
	}
}

VB* vbSubVB(const VB *const vb, const long startIndex, const long endIndex) {
	if (startIndex < 0 || endIndex < 0) {
		vbCustomError("vbSubVB(): indexes out of bound");
	}
	if (endIndex > vb->currentIndex) {
		vbCustomError("vbSubVB(): end index out of bound");
	}
	if (startIndex > endIndex) {
		vbCustomError("vbSubVB(): invblid start/end index");
	}
	if (startIndex == endIndex) {
		return vbNewDetail(vb->typeName, vb->sizeOfElement, 0);
	}

	const long newSize = endIndex - startIndex;
	VB *const newVB = vbMallocVB();
	newVB->currentIndex = newSize;
	newVB->sizeOfBuffer = newSize;
	newVB->typeName = vb->typeName;
	newVB->sizeOfElement = vb->sizeOfElement;
	newVB->buffer = vbMallocBuffer(vb->sizeOfElement * newSize + 1);
	memcpy(newVB->buffer, vb->buffer + startIndex * vb->sizeOfElement, vb->sizeOfElement * newSize);
	vbSetNullByte(newVB);

	return newVB;
}

void vbTypeCastDetail(VB *const vb, const char *const newTypeName, const unsigned long newSizeOfElement) {
	if (vb->sizeOfElement == newSizeOfElement) {
		vb->typeName = newTypeName;
		return;
	}
	if ((vb->currentIndex * vb->sizeOfElement) % newSizeOfElement != 0) {
		vbCustomError("vbTypeCast(): cannot convert from type \"%s\" to type \"%s\"", vb->typeName, newTypeName);
	}

	vb->currentIndex = (vb->currentIndex * vb->sizeOfElement) / newSizeOfElement;
	vb->sizeOfBuffer = (vb->sizeOfBuffer * vb->sizeOfElement) / newSizeOfElement;
	vb->typeName = newTypeName;
	vb->sizeOfElement = newSizeOfElement;
}

long vbFindGeneric(const VB *const vb, const char *const typeName, const void *const element) {
	if (strcmp(vb->typeName, typeName) != 0) {
		vbCustomError("vbFind(): incompatible type \"%s\"", typeName);
	}

	for (const void *i = vb->buffer; i < (vb->buffer + vb->currentIndex * vb->sizeOfElement); i += vb->sizeOfElement) {
		if (memcmp(i, element, vb->sizeOfElement) == 0) return (i - vb->buffer) / vb->sizeOfElement;
	}

	return -1;
}

void vbSort(const VB *const vb, int compare(const void*, const void*)) {
	qsort(vb->buffer, vb->currentIndex, vb->sizeOfElement, compare);
}

void vbForEach(const VB *const vb, void function(void*)) {
	for (void *i = vb->buffer; i < (vb->buffer + vb->currentIndex * vb->sizeOfElement); i += vb->sizeOfElement) {
		function(i);
	}
}