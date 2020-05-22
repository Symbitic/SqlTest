```
file examples/Makefile | grep ASCII &> /dev/null
[ $? -eq 0 ]

for f in $(find examples -type f)
do
  file "$f" | grep "ELF 64-bit LSB executable" &> /dev/null
  if [ $? -eq 0 ]
  then
    echo "$f"
    base=${f#examples/}
    out="out/$base"
    mkdir -p $(dirname "$out")
    cp "$f" "$out"
  fi
done

for f in \
examples/sql/sqlwidgetmapper/sqlwidgetmapper \
examples/sql/sqlbrowser/sqlbrowser \
examples/widgets/widgets/lineedits/lineedits \
examples/widgets/itemviews/editabletreemodel/editabletreemodel \
examples/widgets/itemviews/simplewidgetmapper/simplewidgetmapper \
examples/widgets/itemviews/spreadsheet/spreadsheet \
examples/widgets/itemviews/combowidgetmapper/combowidgetmapper \
examples/widgets/itemviews/basicsortfiltermodel/basicsortfiltermodel \
examples/widgets/itemviews/simpletreemodel/simpletreemodel \
examples/widgets/itemviews/fetchmore/fetchmore \
examples/widgets/itemviews/stardelegate/stardelegate \
examples/widgets/itemviews/chart/chart \
examples/widgets/itemviews/customsortfiltermodel/customsortfiltermodel \
examples/widgets/itemviews/simpledommodel/simpledommodel \
examples/widgets/layouts/flowlayout/flowlayout \
examples/widgets/graphicsview/diagramscene/diagramscene \
examples/widgets/richtext/textedit/textedit \
examples/widgets/richtext/syntaxhighlighter/syntaxhighlighter \
examples/widgets/richtext/orderform/orderform \
examples/widgets/tools/customcompleter/customcompleter \
examples/widgets/tools/i18n/i18n \
examples/widgets/tools/settingseditor/settingseditor \
examples/widgets/mainwindows/sdi/sdi \
examples/widgets/mainwindows/application/application \
examples/xml/saxbookmarks/saxbookmarks \
examples/xml/htmlinfo/htmlinfo \
examples/xml/xmlstreamlint/xmlstreamlint \
examples/xml/streambookmarks/streambookmarks \
examples/xml/dombookmarks/dombookmarks \
examples/xml/rsslisting/rsslisting \
examples/corelib/serialization/savegame/savegame
do
  indir=$(dirname $f)
  outdir=${indir/examples/out2}
  mkdir -p "$outdir"
  cp -r "${indir}" "out2"
done


/opt/Qt/Examples/Qt-5.14.0/location/geojson_viewer/main.cpp
/opt/Qt/Examples/Qt-5.14.0/quickcontrols2/texteditor/documenthandler.h
```

https://doc.qt.io/qt-5/qfile.html
https://doc.qt.io/qt-5/properties.html

**https://doc.qt.io/qt-5/qml-qttest-testcase.html**

```
********* Start testing of MathTests *********
Config: Using QTest library 4.7.2, Qt 4.7.2
PASS   : MathTests::initTestCase()
FAIL!  : MathTests::test_fail() 2 + 2 = 5
   Actual (): 4
   Expected (): 5
   Loc: [/home/.../tst_math.qml(12)]
PASS   : MathTests::test_math()
PASS   : MathTests::cleanupTestCase()
Totals: 3 passed, 1 failed, 0 skipped
********* Finished testing of MathTests *********
```

https://doc.qt.io/qt-5/qml-qtquick-controls2-toolbar.html
https://doc.qt.io/qt-5/scalability.html
https://doc.qt.io/qt-5/qtquick-performance.html
https://doc.qt.io/qt-5/qtquick-bestpractices.html
https://doc.qt.io/qt-5/qmlapplications.html
https://doc.qt.io/qt-5/qtquickcontrols2-material.html

---

```
FocusScope
container.forceActiveFocus()
MenuItem {}
ToolBar {}
DelegateModel
width: GridView.view.cellWidth; height: GridView.view.cellHeight
/opt/Qt/Examples/Qt-5.14.0/positioning/geoflickr/flickr.qml
```

https://www.quora.com/Is-Qt-dying

https://github.com/bluetiger9/SmtpClient-for-Qt
https://github.com/cutelyst/simple-mail
http://www.courier-mta.org/cone/
https://cgit.kde.org/kmailtransport.git
http://www.stafford.uklinux.net/libesmtp/
https://cgit.kde.org/ksmtp.git/
https://curl.haxx.se/libcurl/c/example.html
https://github.com/xcoder123/SimpleSmtp_SSL_QT5
https://www.vmime.org/

`QFINDTESTDATA`
https://code.qt.io/cgit/qt/qtlottie.git/tree/tests/auto/bodymovin/shape/rect/tst_bmrect.cpp
https://code.qt.io/cgit/qt/qttools.git/tree/src/qdoc
https://code.qt.io/cgit/qt/qttools.git/tree/tests/auto/qdoc/generatedoutput/tst_generatedoutput.cpp
`DEFINES SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\"`
https://code.qt.io/cgit/qt/qttools.git/tree/configure.json
https://code.qt.io/cgit/qt/qtdeclarative.git/tree/src/qml/configure.json
https://code.qt.io/cgit/qt/qtbase.git/tree/cmake
https://code.qt.io/cgit/qt/qtbase.git/tree/cmake/FindOracle.cmake
https://code.qt.io/cgit/qt/qtbase.git/tree/configure.json
https://code.qt.io/cgit/qt/qtbase.git/tree/tests/auto/sql/kernel/qsqldatabase/testdata/qtest.mdb
https://code.qt.io/cgit/qt/qtbase.git/tree/tests/auto/sql/kernel/qsqldatabase/tst_databases.h
https://code.qt.io/cgit/qt/qtbase.git/tree/tests/auto/sql/kernel/qsqldatabase/tst_qsqldatabase.cpp

```
if (dbType == QSqlDriver::Oracle)
QFETCH_GLOBAL(QString, dbName);
```

https://code.qt.io/cgit/qt/qtbase.git/tree/tests/auto/sql/kernel/qsqlquery/tst_qsqlquery.cpp
https://code.qt.io/cgit/qt/qtbase.git/tree/tests/auto/sql/kernel/qsql/tst_qsql.cpp
https://doc.qt.io/qt-5/qml-qttest-testcase.html

https://code.qt.io/cgit/qt/qtquickcontrols2.git/tree/examples/quickcontrols2/gallery/pages/TextFieldPage.qml?h=5.14
https://code.qt.io/cgit/qt/qtquickcontrols2.git/tree/examples/quickcontrols2/gallery/pages/DialogPage.qml?h=5.14
https://code.qt.io/cgit/qt/qtquickcontrols2.git/tree/examples/quickcontrols2/gallery/gallery.qml?h=5.14
https://code.qt.io/cgit/qt/qtbase.git/tree/examples/widgets/itemviews/chart/pieview.h?h=5.14
https://code.qt.io/cgit/qt/qtbase.git/tree/examples/widgets/itemviews/chart/pieview.cpp?h=5.14
https://code.qt.io/cgit/qt/qtbase.git/tree/examples/sql?h=5.14
https://doc.qt.io/qt-5/sql-programming.html
https://doc.qt.io/qt-5/topics-data-storage.html
https://doc.qt.io/qt-5/qmlapplications.html
https://doc.qt.io/qt-5/qtquick-views-example.html
https://doc.qt.io/qt-5/qtquickcontrols-flatstyle-example.html
https://doc.qt.io/qt-5/qtquick-righttoleft-example.html
https://stackoverflow.com/questions/18624701/what-support-is-there-for-json-schema-in-qt-c-c
https://code.qt.io/cgit/qt/qtdeclarative.git/tree/examples/qml/referenceexamples/valuesource/example.qml?h=5.14
https://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html
https://doc.qt.io/qt-5.12/qtquick-tableview-pixelator-imagemodel-h.html
https://doc.qt.io/qt-5.12/qtquick-tableview-pixelator-imagemodel-cpp.html
https://doc.qt.io/qt-5/topics-data-storage.html
https://doc.qt.io/qt-5/json.html

https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html
https://en.wikipedia.org/wiki/ANSI_escape_code
http://web.theurbanpenguin.com/adding-color-to-your-output-from-c/

https://doc.qt.io/qt-5/sql-model.html
https://doc.qt.io/qt-5/qsqlquerymodel.html
https://doc.qt.io/qt-5/qsqlresult.html
https://doc.qt.io/qt-5/qsqlrecord.html
https://doc.qt.io/qt-5/qsqlquery.html
https://doc.qt.io/qt-5/qsqlfield.html
https://doc.qt.io/qt-5/qsqldatabase.html
https://github.com/chalk/supports-color/blob/master/.github/security.md
https://github.com/chalk/supports-color/blob/master/.github/funding.yml

https://github.com/tora-tool/tora
http://soci.sourceforge.net/
http://soci.sourceforge.net/doc/master/
https://github.com/jarulraj/sqlcheck
https://github.com/QxOrm/QxOrm
https://github.com/SOCI/soci/blob/master/cmake/modules/FindOracle.cmake
https://specifications.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
icon.name: "application-exit"

```c++
QHash<QUrl, JsonPointer> baseUrlCache;
QHash<JsonPointer, JsonPointer> referenceCache;
const auto& reference = referenceResolver->resolve(schemaPtr);
static QHash<QString, Type> typeHash {
    { "null", TYPE_NULL },
    { "boolean", TYPE_BOOLEAN },
    { "object", TYPE_OBJECT },
    { "array", TYPE_ARRAY },
    { "number", TYPE_NUMBER },
    { "string", TYPE_STRING },
    { "integer", TYPE_INTEGER },
};
```


```cmake
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_DEBUG_POSTFIX d)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

target_compile_definitions(libudoc PRIVATE UDOC_EXPORT)

target_compile_definitions(libudoc PUBLIC
    UDOC_VERSION=${PROJECT_VERSION}
)

set_target_properties(libudoc PROPERTIES OUTPUT_NAME udoc)

set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
```
https://cmake.org/cmake/help/latest/command/find_package.html
https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html
https://github.com/Symbitic/tree-sitter-cmake/blob/master/CMakeLists.txt
https://github.com/Symbitic/tree-sitter-cmake/blob/master/tree-sitter-config.cmake
https://bitbucket.org/gallen/libvariant/src/default/LibVariantConfig.cmake.in
https://bitbucket.org/gallen/libvariant/src/default/Config.h.in

https://code.qt.io/cgit/qt/qtbase.git/tree/examples/sql/books/initdb.h?h=5.14
https://code.qt.io/cgit/qt/qtbase.git/tree/examples/sql/drilldown/view.cpp?h=5.14
https://code.qt.io/cgit/qt/qtbase.git/tree/examples/sql/masterdetail/dialog.cpp?h=5.14
https://code.qt.io/cgit/qt/qtbase.git/tree/examples/sql/sqlbrowser/browser.cpp?h=5.14
`QObject::connect(&d->json, &QSqlTestJsonFile::error, [this](const QString &err) { this->emit error(err); });`
```
connect(window, QOverload<int,const QString &>::of(&InformationWindow::imageChanged), this, QOverload<int,const QString &>::of(&View::updateImage));
```


https://cmake.org/cmake/help/latest/command/install.html
https://cmake.org/cmake/help/latest/command/export.html
https://cmake.org/cmake/help/latest/command/option.html
https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html
https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html

```
make && ./sqltestrunner ../tests/fixtures/superheroes.json
```
