mkdir c:\lpg\msvc\locale
mkdir c:\lpg\msvc\locale\fr
mkdir c:\lpg\msvc\locale\fr\LC_MESSAGES

msgfmt -c -o fr.mo fr.po
copy fr.mo c:\lpg\msvc\locale\fr\LC_MESSAGES\tilp2.mo

msgfmt -c -o ../../ticables2/po/fr.mo ../../ticables2/po/fr.po
copy ..\..\ticables2\po\fr.mo c:\lpg\msvc\locale\fr\LC_MESSAGES\libticables2.mo

msgfmt -c -o ../../tifiles2/po/fr.mo ../../tifiles2/po/fr.po
copy ..\..\tifiles2\po\fr.mo c:\lpg\msvc\locale\fr\LC_MESSAGES\libtifiles2.mo

msgfmt -c -o ../../ticalcs2/po/fr.mo ../../ticalcs2/po/fr.po
copy ..\..\ticalcs2\po\fr.mo c:\lpg\msvc\locale\fr\LC_MESSAGES\libticalcs2.mo
