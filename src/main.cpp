#include <libtomcrypt/tomcrypt.h>

#include <QtWidgets/QApplication>
#include <QtGui/QStyleHints>

#include "window.hpp"

int main(int argc, char *argv[])
{
    # ifdef USE_LTM
        ltc_mp = ltm_desc;
    # elif defined(USE_TFM)
        ltc_mp = tfm_desc;
    # endif

    QApplication app(argc, argv);

    app.setApplicationName("Voleur");
    app.setStyle("fusion");

    QStyleHints* style = app.styleHints();

    VoleurWindow window = VoleurWindow();
    window.show();

    return app.exec();
}
