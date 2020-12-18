#ifndef EXCEPTIONS_H
#define EXCEPTION_H
namespace EXCEPTIONS {
	enum class FILE {
		PATH_NOT_FOUND

	};
	enum class HASH {
		FAILED_CREATING_STATE,
		FAILED_INITIALIZING_STATE,
		FAILED_UPDATING_HASH
	};
};
#endif