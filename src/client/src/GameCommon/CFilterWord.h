#ifndef _CFilterWord_
#define _CFilterWord_
#include <map>
#include <string>

class CFilterWord {
  CFilterWord(void );
  ~CFilterWord(void);
public:
  static CFilterWord& GetInstance();

  void AddWord(char* pszWord, char* pszChangeWord);
  void AddName(char* pszName);

  void SetChangeWord(const char* pszWord);
  bool CheckString(char*         pszString);
  bool CheckName(char*           pszName);

  std::string& GetChangedString();
  std::string& GetCheckedWord();

  int MulityByte2WideString(const char* pszMultyByte, std::wstring& wstrWide);
  int MulityByte2WideString(const char* pszMultyByte, wchar_t* pwszWide, int iBufSize);

  int Wide2MultyByteString(std::wstring& wstrWide, std::string& strMultyByte);

  bool IsEnglish(wchar_t  wchar);
  bool IsNumber(wchar_t   wchar);
  bool IsKorean(wchar_t   wchar);
  bool IsJapanese(wchar_t wchar);
  bool IsHanja(wchar_t    wchar);

  bool IsValidName(char*           szName);
  bool CheckKorean(std::wstring&   wstrName);
  bool CheckJapanese(std::wstring& wstrName);
  bool CheckEnglish(std::wstring&  wstrName);
  bool CheckChinese(std::wstring&  wstrName);

private:

  std::string m_strCheckedWord;
  std::string m_strChangedString;

  std::wstring m_wstrChangeWord;

  std::map<std::wstring, std::wstring> m_BadWords;
  std::list<std::wstring>         m_BadNames;
};
#endif
