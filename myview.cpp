#include "myview.h"
#include "mybox.h"
#include <QtGui/QIcon>
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QApplication>
#include <QtGui/QLabel>
//#include <FileInfo>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QGraphicsBlurEffect>
#include <QtCore/QTimer>

static const qreal INITSPEED=500;

MyView::MyView(QWidget *parent)
	: QGraphicsView(parent)
{
	initView();
}

MyView::~MyView()
{

}

//��ʼ����Ϸ����
void MyView::initView()
{
	//ʹ�ÿ������Ⱦ
	setRenderHint(QPainter::Antialiasing);
	//���û��汳�����������Լӿ���Ⱦ�ٶ�
	setCacheMode(CacheBackground);
	setWindowTitle(tr("MyBox������Ϸ"));
	setWindowIcon(QIcon("./images/icon.png"));
	setMinimumSize(410,510);
	setMaximumSize(410,510);

	//���ó���
	QGraphicsScene *scene=new QGraphicsScene;
	scene->setSceneRect(5,5,400,500);
    scene->setBackgroundBrush(QPixmap(":/File/images/background.png"));
	setScene(scene);

	//������ƶ������4���߽���
	topLine=scene->addLine(27,47,233,47);
	bottomLine=scene->addLine(27,453,233,453);
	leftLine=scene->addLine(27,47,27,453);
	rightLine=scene->addLine(233,47,233,453);

	//��ǰ�������ʾ������
	boxGroup=new BoxGroup;
	connect(boxGroup,SIGNAL(needNewBox()),this,SLOT(clearFullRows()));
	connect(boxGroup,SIGNAL(gameFinished()),this,SLOT(gameOver()));
	scene->addItem(boxGroup);
	nextBoxGroup=new BoxGroup;
	scene->addItem(nextBoxGroup);

	//����
	/*QLabel *scoreLabel = new QLabel(this);
	scoreLabel->setText(tr("<h1><font color=black>����:</font></h1>"));
	scoreLabel->setAlignment(Qt::AlignCenter);
	scoreLabel->move(240, 125);*/

	gameScoreText = new QGraphicsTextItem(0, scene);
	gameScoreText->setFont(QFont("Times", 20, QFont::Bold));
	gameScoreText->setPos(300, 120);

	//// ���ó�ʼΪ����״̬
	topLine->hide();
	bottomLine->hide();
	leftLine->hide();
	rightLine->hide();
	gameScoreText->hide();

	//// ��ɫ����
	QWidget *mask = new QWidget;
	mask->setAutoFillBackground(true);
	mask->setPalette(QPalette(QColor(0, 60, 60, 80)));
	mask->resize(900, 600);
	maskWidget = scene->addWidget(mask);
	maskWidget->setPos(-50, -50);
	// ������ZֵΪ1���������Դ���ZֵΪ0��ͼ��������
	maskWidget->setZValue(1);

	//// ѡ�����
	QWidget *option = new QWidget;
	QPushButton *optionCloseButton = new QPushButton(tr("��   ��"), option);
	QPalette palette;
	palette.setColor(QPalette::ButtonText, Qt::black);
	optionCloseButton->setPalette(palette);
	optionCloseButton->move(110, 250);
	connect(optionCloseButton, SIGNAL(clicked()), option, SLOT(hide()));
	option->setAutoFillBackground(true);
	option->setPalette(QPalette(QColor(0, 0, 0, 180)));
	option->resize(300, 300);
	QGraphicsWidget *optionWidget = scene->addWidget(option);
	optionWidget->setPos(60, 100);
	optionWidget->setZValue(3);
	optionWidget->hide();

	//// �������
	QWidget *help = new QWidget;
	QPushButton *helpCloseButton = new QPushButton(tr("��   ��"), help);
	helpCloseButton->setPalette(palette);
	helpCloseButton->move(110, 250);
	connect(helpCloseButton, SIGNAL(clicked()), help, SLOT(hide()));
	help->setAutoFillBackground(true);
	help->setPalette(QPalette(QColor(0, 0, 0, 180)));
	help->resize(300, 300);
	QGraphicsWidget *helpWidget = scene->addWidget(help);
	helpWidget->setPos(60, 100);
	helpWidget->setZValue(3);
	helpWidget->hide();

	QLabel *helpLabel = new QLabel(help);
	helpLabel->setText(tr("<h1><font color=white>������Ϸ</font></h1>"));
	helpLabel->setAlignment(Qt::AlignCenter);
	helpLabel->move(95, 90);

	//// ��Ϸ��ӭ�ı�
	gameWelcomeText = new QGraphicsTextItem(0, scene);
	gameWelcomeText->setHtml(tr("<font color=white>����˹������Ϸ</font>"));
	gameWelcomeText->setFont(QFont("Times", 30, QFont::Bold));
	gameWelcomeText->setPos(60, 100);
	gameWelcomeText->setZValue(2);

	// ��Ϸ��ͣ�ı�
	gamePausedText = new QGraphicsTextItem(0, scene);
	gamePausedText->setHtml(tr("<font color=white>��Ϸ��ͣ�У�</font>"));
	gamePausedText->setFont(QFont("Times", 30, QFont::Bold));
	gamePausedText->setPos(110, 100);
	gamePausedText->setZValue(2);
	gamePausedText->hide();

	// ��Ϸ�����ı�
	gameOverText = new QGraphicsTextItem(0, scene);
	gameOverText->setHtml(tr("<font color=white>��Ϸ������</font>"));
	gameOverText->setFont(QFont("Times", 30, QFont::Bold));
	gameOverText->setPos(110, 100);
	gameOverText->setZValue(2);
	gameOverText->hide();

	QPushButton *button1 = new QPushButton(tr("��    ʼ"));
	QPushButton *button2 = new QPushButton(tr("ѡ    ��"));
	QPushButton *button3 = new QPushButton(tr("��    ��"));
	QPushButton *button4 = new QPushButton(tr("��    ��"));
	QPushButton *button5 = new QPushButton(tr("���¿�ʼ"));
	QPushButton *button6 = new QPushButton(tr("��    ͣ"));
	QPushButton *button7 = new QPushButton(tr("�� �� ��"));
	QPushButton *button8 = new QPushButton(tr("������Ϸ"));
	QPushButton *button9 = new QPushButton(tr("������Ϸ"));

	connect(button1, SIGNAL(clicked()), this, SLOT(startGame()));
	connect(button2, SIGNAL(clicked()), option, SLOT(show()));
	connect(button3, SIGNAL(clicked()), help, SLOT(show()));
	connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
	connect(button5, SIGNAL(clicked()), this, SLOT(restartGame()));
	connect(button6, SIGNAL(clicked()), this, SLOT(pauseGame()));
	connect(button7, SIGNAL(clicked()), this, SLOT(finishGame()));
	connect(button8, SIGNAL(clicked()), this, SLOT(returnGame()));
	connect(button9, SIGNAL(clicked()), this, SLOT(finishGame()));

	startButton = scene->addWidget(button1);
	optionButton = scene->addWidget(button2);
	helpButton = scene->addWidget(button3);
	exitButton = scene->addWidget(button4);
	restartButton = scene->addWidget(button5);
	pauseButton = scene->addWidget(button6);
	showMenuButton = scene->addWidget(button7);
	returnButton = scene->addWidget(button8);
	finishButton = scene->addWidget(button9);

	startButton->setPos(170, 200);
	optionButton->setPos(170, 250);
	helpButton->setPos(170, 300);
	exitButton->setPos(170, 350);
	restartButton->setPos(260, 290);
	pauseButton->setPos(260, 340);
	showMenuButton->setPos(260, 390);
	returnButton->setPos(170, 200);
	finishButton->setPos(160, 250);

	startButton->setZValue(2);
	optionButton->setZValue(2);
	helpButton->setZValue(2);
	exitButton->setZValue(2);
	restartButton->setZValue(2);
	returnButton->setZValue(2);
	finishButton->setZValue(2);

	restartButton->hide();
	finishButton->hide();
	pauseButton->hide();
	showMenuButton->hide();
	returnButton->hide();

	backgroundMusic = new Phonon::MediaObject(this);
	clearRowSound = new Phonon::MediaObject(this);
	Phonon::AudioOutput *audio1 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	Phonon::AudioOutput *audio2 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	Phonon::createPath(backgroundMusic, audio1);
	Phonon::createPath(clearRowSound, audio2);
    Phonon::VolumeSlider *volume1 = new Phonon::VolumeSlider(audio1, option);
    Phonon::VolumeSlider *volume2 = new Phonon::VolumeSlider(audio2, option);
	QLabel *volumeLabel1 = new QLabel(tr("����:"), option);
	QLabel *volumeLabel2 = new QLabel(tr("��Ч:"), option);
    volume1->move(10, 100);
    volume2->move(10, 150);
	volumeLabel1->move(60, 80);
	volumeLabel2->move(60, 130);
	connect(backgroundMusic, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
	connect(clearRowSound, SIGNAL(finished()), clearRowSound, SLOT(stop()));
    backgroundMusic->setCurrentSource(Phonon::MediaSource(":/File/sounds/background.mp3"));
    clearRowSound->setCurrentSource(Phonon::MediaSource(":/File/sounds/clearRow.mp3"));
	backgroundMusic->play();

}

//��ʼ��Ϸ
void MyView::startGame()
{
	gameWelcomeText->hide();
	startButton->hide();
	optionButton->hide();
	helpButton->hide();
	exitButton->hide();
	maskWidget->hide();
	initGame();
}

// ��ʼ����Ϸ
void MyView::initGame()
{
    backgroundMusic->setCurrentSource(Phonon::MediaSource(":/File/sounds/background.mp3"));
	backgroundMusic->play();

	gameScoreText->setHtml(tr("<font color = black>0</font>"));
	boxGroup->createBox(QPointF(130, 70));
	boxGroup->setFocus();
	boxGroup->startTimer(INITSPEED);
	gameSpeed = INITSPEED;
	nextBoxGroup->createBox(QPointF(300, 70));

	topLine->show();
	bottomLine->show();
	leftLine->show();
	rightLine->show();
	gameScoreText->show();
	restartButton->show();
	pauseButton->show();
	showMenuButton->show();
	boxGroup->show();
}

// �������
void MyView::clearFullRows()
{
	// ��ȡ��һ�з���ϴ�ľ����а���������С����
	for (int y = 429; y > 50; y -= 20) {
		QList<QGraphicsItem *> list = scene()->items(28, y, 203, 22, Qt::ContainsItemShape);
		// �����������
		if (list.count() == 10) {
			foreach (QGraphicsItem *item, list) {
				OneBox *box = (OneBox*) item;
				QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect;
				box->setGraphicsEffect(blurEffect);
				QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");
				animation->setEasingCurve(QEasingCurve::OutBounce);
				animation->setDuration(250);
				animation->setStartValue(4);
				animation->setEndValue(0.25);
				animation->start(QAbstractAnimation::DeleteWhenStopped);
				connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));
			}
			// �������е�λ��
			rows << y;
		}
	}
	// ��������У�������������ĸ����ٳ����µķ�����
	// ���û�����У���ֱ�ӳ����µķ�����
	if(rows.count() > 0) {
		clearRowSound->play();
		QTimer::singleShot(400, this, SLOT(moveBox()));
	} else {
		boxGroup->createBox(QPointF(130, 70), nextBoxGroup->getCurrentShape());
		// ��ղ�������ʾ�������е�����С����
		nextBoxGroup->clearBoxGroup(true);
		nextBoxGroup->createBox(QPointF(300, 70));
	}
}

// �����������������С����
void MyView::moveBox()
{
	// ��λ����ϵ����п�ʼ
	for (int i = rows.count(); i > 0;  --i) {
		int row = rows.at(i - 1);
		foreach (QGraphicsItem *item, scene()->items(29, 49, 202, row - 47, Qt::ContainsItemShape)) {
			item->moveBy(0, 20);
		}
	}
	// ���·���
	updateScore(rows.count());
	// �������б����Ϊ0
	rows.clear();
	// �������������Ժ��ٳ����µķ�����
	boxGroup->createBox(QPointF(130, 70), nextBoxGroup->getCurrentShape());
	nextBoxGroup->clearBoxGroup(true);
	nextBoxGroup->createBox(QPointF(300, 70));
}


void MyView::updateScore(const int fullRowNum)
{
	// ���·���
	int score = fullRowNum * 10;
	int currentScore = gameScoreText->toPlainText().toInt();
	currentScore += score;
	gameScoreText->setHtml(tr("<font color = black>%1</font>").arg(currentScore));
    if(currentScore < 500)
	{}
    else if(currentScore < 1000)
	{
		gameSpeed = 300;
		boxGroup->stopTimer();
		boxGroup->startTimer(gameSpeed);
        backgroundMusic->play();
	}
	else
	{
		gameSpeed = 200;
		boxGroup->stopTimer();
		boxGroup->startTimer(gameSpeed);
        backgroundMusic->play();
	}
}

void MyView::gameOver()
{
	// ��Ϸ����
	pauseButton->hide();
	showMenuButton->hide();
	maskWidget->show();
	gameOverText->show();
	restartButton->setPos(160, 200);
	finishButton->show();
}

void MyView::restartGame()
{
	maskWidget->hide();
	gameOverText->hide();
	finishButton->hide();
	restartButton->setPos(260, 290);

	// ������ʾ������͵�ǰ�����ƶ������е�����С����
	nextBoxGroup->clearBoxGroup(true);
	boxGroup->clearBoxGroup();
	boxGroup->hide();
	foreach (QGraphicsItem *item, scene()->items(29, 49, 202, 402, Qt::ContainsItemShape)) {
		// �ȴӳ������Ƴ�С���飬��Ϊʹ��deleteLater()���ڷ������¼�ѭ���������
		// С����ģ�Ϊ���ڳ����µķ�����ʱ��������ײ��������Ҫ�ȴӳ������Ƴ�С����
		scene()->removeItem(item);
		OneBox *box = (OneBox*) item;
		box->deleteLater();
	}

	initGame();
}

void MyView::finishGame()
{
	gameOverText->hide();
	finishButton->hide();
	restartButton->setPos(260, 290);
	restartButton->hide();
	pauseButton->hide();
	showMenuButton->hide();
	gameScoreText->hide();

	topLine->hide();
	bottomLine->hide();
	leftLine->hide();
	rightLine->hide();

	nextBoxGroup->clearBoxGroup(true);
	boxGroup->clearBoxGroup();
	boxGroup->hide();

	foreach (QGraphicsItem *item, scene()->items(29, 49, 202, 402, Qt::ContainsItemShape)) {
		OneBox *box = (OneBox*) item;
		box->deleteLater();
	}

	// �������ڽ�����Ϸʱ���¡����˵�����ť
	maskWidget->show();
	gameWelcomeText->show();
	startButton->show();
	optionButton->show();
	helpButton->show();
	exitButton->show();
    scene()->setBackgroundBrush(QPixmap(":/File/images/background.png"));
}

void MyView::returnGame()
{
	returnButton->hide();
	gamePausedText->hide();
	maskWidget->hide();
	restartButton->show();
	pauseButton->show();
	showMenuButton->show();
	boxGroup->startTimer(gameSpeed);
}

void MyView::pauseGame()
{
	boxGroup->stopTimer();
	restartButton->hide();
	pauseButton->hide();
	showMenuButton->hide();
	maskWidget->show();
	gamePausedText->show();
	returnButton->show();
}

void MyView::keyPressEvent(QKeyEvent *event)
{
	if (pauseButton->isVisible())
		boxGroup->setFocus();
	else
		boxGroup->clearFocus();
	QGraphicsView::keyPressEvent(event);
}

void MyView::aboutToFinish()
{
	backgroundMusic->enqueue(backgroundMusic->currentSource());
}