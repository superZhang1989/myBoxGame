#ifndef MYVIEW_H
#define MYVIEW_H

#include <QtGui/QGraphicsView>
#include <phonon/phonon>
class BoxGroup;

class MyView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit MyView(QWidget *parent=0);
	~MyView();

public slots:
	void startGame();
	void clearFullRows();
	void moveBox();
	void gameOver();
	void restartGame();
	void finishGame();
	void pauseGame();
	void returnGame();
	void aboutToFinish();

private:
	BoxGroup *boxGroup;
	BoxGroup *nextBoxGroup;

	QGraphicsWidget *maskWidget;
	QGraphicsWidget *startButton;
	QGraphicsWidget *optionButton;
	QGraphicsWidget *helpButton;
	QGraphicsWidget *exitButton;
	QGraphicsWidget *finishButton;
	QGraphicsWidget *restartButton;
	QGraphicsWidget *pauseButton;
	QGraphicsWidget *returnButton;
	QGraphicsWidget *showMenuButton;

	QGraphicsLineItem *topLine;
	QGraphicsLineItem *bottomLine;
	QGraphicsLineItem *leftLine;
	QGraphicsLineItem *rightLine;

	QGraphicsTextItem *gameScoreText;
	QGraphicsTextItem *gameWelcomeText;
	QGraphicsTextItem *gamePausedText;
	QGraphicsTextItem *gameOverText;

	qreal gameSpeed;
	QList<int> rows;
	void initView();
	void initGame();
	void updateScore(const int fullRowNum=0);

	Phonon::MediaObject *backgroundMusic;
	Phonon::MediaObject *clearRowSound;

protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif // MYVIEW_H
