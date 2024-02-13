#include "jitbit.hpp"
#include "window.hpp"

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 120

VoleurWindow::VoleurWindow() : QWidget(){
	this->setFixedSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT));

	this->licensorInput = new QLineEdit(this);
	this->licensorInput->move(QPoint(16, 16));
	this->licensorInput->setFixedSize(QSize(WINDOW_WIDTH - 32, 25));
	this->licensorInput->setPlaceholderText("Licensor");

	this->generateButton = new QPushButton(this);
	this->generateButton->move(QPoint((WINDOW_WIDTH - 100) / 2, 57));
	this->generateButton->setFixedSize(QSize(100, 25));
	this->generateButton->setText("Generate Key");
	this->connect(this->generateButton, &QPushButton::released, this, &VoleurWindow::handleGenerateButton);

	this->statusText = new QLabel(this);
	this->statusText->move(QPoint(0, 82));
	this->statusText->setFixedSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT - 82));
	this->statusText->setAlignment(Qt::AlignCenter);
	this->statusText->setText("");
}

void VoleurWindow::handleGenerateButton() {
	std::string text = this->licensorInput->text().toStdString();
	if(text.length() == 0){
		this->showError("Enter a licensor name. This can be anything");
		return;
	}
	std::string result = crack_jitbit(text.c_str());
	if(result.length() != 0){
		this->showError(result.c_str());
		return;
	}
	this->showSuccess("Success! Happy macro-ing!");
}

void VoleurWindow::showError(const QString& str){
	QPalette palette = this->statusText->palette();
	palette.setColor(QPalette::WindowText, Qt::red);
	this->statusText->setPalette(palette);
	this->statusText->setText(str);
}

void VoleurWindow::showSuccess(const QString& str){
	QPalette palette = this->statusText->palette();
	palette.setColor(QPalette::WindowText, Qt::green);
	this->statusText->setPalette(palette);
	this->statusText->setText(str);
}