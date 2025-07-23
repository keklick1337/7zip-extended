// OpenCallbackConsole.h

#ifndef ZIP7_INC_OPEN_CALLBACK_CONSOLE_H
#define ZIP7_INC_OPEN_CALLBACK_CONSOLE_H

#include "../../../Common/StdOutStream.h"

#include "../Common/ArchiveOpenCallback.h"

#include "PercentPrinter.h"

// by abc321 \/
#ifndef Z7_NO_CRYPTO
// by abc321 /\~

#include <iostream>
#include <fstream>
#include <string>

// by abc321 \/
#include <locale>
#include <codecvt>
#include <cstdlib>

class CPasswordReader
{
private:
#ifndef Z7_ASCII_PASSWORD_FILE
	std::wifstream PasswordFile;
#else
	std::ifstream PasswordFile;  // ASCII file
	//std::basic_ifstream<char32_t> PasswordFile;
#endif
public:
	CPasswordReader(UString PasswordFileName) {
#ifndef Z7_ASCII_PASSWORD_FILE
#ifdef _MSC_VER
		const std::locale empty_locale = std::locale::empty();
#else
		const std::locale empty_locale = std::locale();
#endif
		typedef std::codecvt_utf8_utf16<wchar_t> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_utf16_locale = std::locale(empty_locale, converter);
#ifdef _MSC_VER
		PasswordFile.open(PasswordFileName);
#else
		std::wstring wFileName(PasswordFileName);
		std::string fileName(wFileName.begin(), wFileName.end());
		PasswordFile.open(fileName);
#endif
		if (PasswordFile.is_open())
			PasswordFile.imbue(utf8_utf16_locale);
#else
		PasswordFile.open(PasswordFileName); // ASCII file
		//PasswordFile.open(PasswordFileName, std::ios::in | std::ios::binary);
#endif
	}

	~CPasswordReader() {
		if (PasswordFile)
			if (PasswordFile.is_open())
				PasswordFile.close();
	}

	HRESULT GetNextPassword(UString *Password) {
#ifndef Z7_ASCII_PASSWORD_FILE
		/*if (auto f = std::wofstream(L"C:\\Users\\ADMINI~1.YSE\\DOCUME~1\\VISUAL~1\\Projects\\7Z2407~1\\_test\\125.txt")) {
			f.imbue(std::locale(std::locale(),
				new std::codecvt_utf8_utf16<wchar_t>)); // assumes wchar_t is UTF-16
			f << L"кошка 日本国\n";
			f << L"𝟐𝟔𝟎𝐤/\n";
		}*/

		std::wstring line;
		*Password = "";
		if (PasswordFile.is_open()) {
			unsigned i = 0;
			while (i == 0) {
				if (std::getline(PasswordFile, line)) {
					for (i = 0; ((line[i] != 0) && (line[i] != U'\r')); i++);
					if ((i > 0) && (line[i] == U'\r')) {
						line[i] = 0;
						i--;
					}
					if (i > 0) {
						wchar_t  *ws = &line[0]; // line.c_str();
						*Password = ws;
					}

				}
				else {
#ifdef _MSC_VER
					return ERROR_END_OF_MEDIA;
#else
					return SZ_ERROR_FAIL;
#endif
				}
			}
		}
		else {
#ifdef _MSC_VER
			return ERROR_FILE_INVALID;
#else
			return ERROR_FILE_NOT_FOUND;
#endif
		}
		return S_OK;
#else
		std::string line;
		//std::u32string line{};
		char buffer[2500];
		*Password = "";
		if (PasswordFile.is_open()) {
			int i = 0;
			while (i == 0) {
				if (PasswordFile.getline(buffer, sizeof(buffer), U'\n')) {
					for (i = 0; (i < sizeof(buffer)) && (buffer[i] != 0) && (buffer[i] != U'\r'); i++) {}
					if (i > 0) {
						*Password = buffer;
					}
				}
				else {
#ifdef _MSC_VER
					return ERROR_END_OF_MEDIA;
#else
					return SZ_ERROR_FAIL;
#endif
				}
			}
		}
		else {
#ifdef _MSC_VER
			return ERROR_FILE_INVALID;
#else
			return ERROR_FILE_NOT_FOUND;
#endif
		}
		return S_OK;
#endif
	};
};
#endif
// by abc321 /\~

class COpenCallbackConsole: public IOpenCallbackUI
{
protected:
  CPercentPrinter _percent;

  CStdOutStream *_so;
  CStdOutStream *_se;

  // UInt64 _totalFiles;
  UInt64 _totalBytes;
  bool _totalFilesDefined;
  // bool _totalBytesDefined;

  bool NeedPercents() const { return _percent._so && !_percent.DisablePrint; }

public:

  bool MultiArcMode;

  void ClosePercents()
  {
    if (NeedPercents())
      _percent.ClosePrint(true);
  }

  COpenCallbackConsole():
      _totalBytes(0),
      _totalFilesDefined(false),
      // _totalBytesDefined(false),
      MultiArcMode(false)
      
      #ifndef Z7_NO_CRYPTO
      , PasswordIsDefined(false)
      // , PasswordWasAsked(false)
	  , PasswordReader(NULL) // by abc321
	  , PasswordBruteforced(false) // by abc321
	  , PasswordPrinted(false) // by abc321
      #endif
      
      {}

  virtual ~COpenCallbackConsole() {}
  
  void Init(
      CStdOutStream *outStream,
      CStdOutStream *errorStream,
      CStdOutStream *percentStream,
      bool disablePercents)
  {
    _so = outStream;
    _se = errorStream;
    _percent._so = percentStream;
    _percent.DisablePrint = disablePercents;
  }

  Z7_IFACE_IMP(IOpenCallbackUI)
  
  #ifndef Z7_NO_CRYPTO
  bool PasswordIsDefined;
  // bool PasswordWasAsked;
  UString Password;
  CPasswordReader *PasswordReader; // by abc321
  bool PasswordBruteforced; // by abc321
  bool PasswordPrinted; // by abc321
  #endif
};

#endif
