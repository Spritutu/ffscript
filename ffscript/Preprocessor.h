#pragma once
#include <string>
#include <memory>

class Preprocessor
{
public:
	Preprocessor();
	virtual ~Preprocessor();

	virtual std::shared_ptr<std::wstring> preprocess(const wchar_t* begin, const wchar_t* end) = 0;
	virtual void getOriginalPosition(int charIndex, int& line, int& column) const = 0;
};

typedef std::shared_ptr<Preprocessor> PreprocessorRef;
