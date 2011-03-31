mkdir ".\Installer\system32.x86"
mkdir ".\Installer\system32.x64
mkdir ".\Installer\OpenVanilla"
mkdir ".\Installer\OpenVanilla\Modules"
mkdir ".\Installer\OpenVanilla\Modules\x86"
mkdir ".\Installer\OpenVanilla\Modules\x64"
mkdir ".\Installer\OpenVanilla\Modules\OVIMArray"
mkdir ".\Installer\OpenVanilla\Modules\OVIMGeneric"
mkdir ".\Installer\OpenVanilla\Modules\OVIMRomanNew"
mkdir ".\Installer\OpenVanilla\en-US"
mkdir ".\Installer\OpenVanilla\zh-TW"
copy "..\..\Dep\x86\libltdl3.dll" .\Installer\system32.x86\
copy "..\..\Dep\x86\libhunspell.dll" .\Installer\system32.x86\
copy "..\..\Dep\x64\libltdl-3.dll" .\Installer\system32.x64\
copy "..\..\Dep\x64\libhunspell.dll" .\Installer\system32.x64\
copy ".\OVManagedUI\bin\Release\OVManagedUI.dll" .\Installer\OpenVanilla\
copy ".\OVManagedUI\bin\Release\OVManagedUI.dll" .\Installer\OpenVanilla\
copy "..\..\Utilities\GacUtil\bin\Release\GacUtil.exe" .\Installer\OpenVanilla\
copy .\OVIMEUI\Release\x86\OVIMEUI.dll .\Installer\system32.x86\
copy .\OVIMEUI\Release\x64\OVIMEUI.dll .\Installer\system32.x64\
copy .\OVIME\Release\x86\OVIME.ime .\Installer\system32.x86\
copy .\OVIME\Release\x64\OVIME.ime .\Installer\system32.x64\
copy .\OVPreferences\bin\Release\OVPreferences.exe .\Installer\OpenVanilla\
copy .\OVPreferences\bin\Release\zh-TW\* .\Installer\OpenVanilla\zh-TW
copy .\OVPreferences\bin\Release\en-US\* .\Installer\OpenVanilla\en-US
copy "..\..\Modules\OVIMArray\Release\x86\OVIMArray.dll" ".\Installer\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMArray\Release\x64\OVIMArray.dll" ".\Installer\OpenVanilla\Modules\x64\"
copy "..\..\Modules\SharedData\array30.cin" ".\Installer\OpenVanilla\Modules\OVIMArray\"
copy "..\..\Modules\SharedData\array-special.cin" ".\Installer\OpenVanilla\Modules\OVIMArray\"
copy "..\..\Modules\SharedData\array-shortcode.cin" ".\Installer\OpenVanilla\Modules\OVIMArray\"
copy "..\..\Modules\OVIMGenericSQ\Release\x86\OVIMGenericSQ.dll" ".\Installer\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMGenericSQ\Release\x64\OVIMGenericSQ.dll" ".\Installer\OpenVanilla\Modules\x64\"
rem copy "..\..\Modules\SharedData\dayi3-patched.cin" ".\Installer\OpenVanilla\Modules\OVIMGeneric\"
rem copy "..\..\Modules\SharedData\ehq-symbols.cin" ".\Installer\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\Modules\OVIMRomanNew\Release\x86\OVIMRomanNew.dll" ".\Installer\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMRomanNew\Release\x64\OVIMRomanNew.dll" ".\Installer\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVIMRomanNew\en_US.aff" ".\Installer\OpenVanilla\Modules\OVIMRomanNew\"
copy "..\..\Modules\OVIMRomanNew\en_US.dic" ".\Installer\OpenVanilla\Modules\OVIMRomanNew\"
copy "..\..\Modules\OVIMPhonetic\Release\x86\OVIMPhoneticStatic.dll" ".\Installer\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVIMPhonetic\Release\x64\OVIMPhoneticStatic.dll" ".\Installer\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVKPPhraseTools\Release\x86\OVKPPhraseTools.dll" ".\Installer\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVKPPhraseTools\Release\x64\OVKPPhraseTools.dll" ".\Installer\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVOFFullWidthCharacter\Release\x86\OVOFFullWidthCharacter.dll" ".\Installer\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVOFFullWidthCharacter\Release\x64\OVOFFullWidthCharacter.dll" ".\Installer\OpenVanilla\Modules\x64\"
copy "..\..\Modules\OVOFHanConvert\Release\x86\OVOFHanConvert.dll" ".\Installer\OpenVanilla\Modules\x86\"
copy "..\..\Modules\OVOFHanConvert\Release\x64\OVOFHanConvert.dll" ".\Installer\OpenVanilla\Modules\x64\"

pause
