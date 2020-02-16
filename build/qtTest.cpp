#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv){

	QApplication app (argc, argv);

	QPushButton button;
	button.setText("My text");
	button.setToolTip("A tooltip");
	button.show();
	
	QWidget window;
    window.resize(640, 640);
    window.show();
    window.setWindowTitle(
    QApplication::translate("toplevel", "Top-level widget"));
	
	return app.exec();
}