
#include "stdafx.h"
#include "wdgAxisValue.h"

wdgAxisValue::wdgAxisValue(QWidget *parent){

	setParent(parent);

	valInterval_.first = 0;
	valInterval_.second = 1000;

	scale_ = (valInterval_.second - valInterval_.first) / height();

}

wdgAxisValue::~wdgAxisValue(){}

void wdgAxisValue::mouseMoveEvent(QMouseEvent * event){

	int pos = event->pos().y(), diff = pos - mousePrevPosY_;

	curOffsPos_ += diff;

	if (curOffsPos_ > curDashStep_) curOffsPos_ = 0;
	if (curOffsPos_ < 0) curOffsPos_ = curDashStep_;

	valInterval_.first += scale_ * diff;
	valInterval_.second += scale_ * diff;

	curInterv_ = valInterval_.second - valInterval_.first;

	mousePrevPosY_ = pos;

	emit req_axisChange();

}

void wdgAxisValue::mousePressEvent(QMouseEvent * event){

	mousePrevPosY_ = event->pos().y();
}

void wdgAxisValue::scale(int delta){
		
	if (delta > 0) curDashStep_++;
	else curDashStep_--;

	if (curDashStep_ > cng_maxDashStep_) curDashStep_ = cng_minDashStep_;
	else if (curDashStep_ < cng_minDashStep_) curDashStep_ = cng_maxDashStep_;

	int offs = 10;

	if (curInterv_ > 1000) offs *= 10;
	else if (curInterv_ > 10000) offs *= 100;
	else if (curInterv_ < 100) offs /= 10;

	if (delta > 0){ // ����������
		valInterval_.first += offs;
		valInterval_.second -= offs;

		if (valInterval_.first >= valInterval_.second) valInterval_.first = valInterval_.second - 0.1;
	}
	else{ // ������
		valInterval_.first -= offs;
		valInterval_.second += offs;
	}

	curInterv_ = valInterval_.second - valInterval_.first;

	scale_ = (valInterval_.second - valInterval_.first) / height();

	emit req_axisChange();
}

void wdgAxisValue::wheelEvent(QWheelEvent * event){

	scale(event->delta());

}

void wdgAxisValue::resizeEvent(QResizeEvent * event){

	scale_ = (valInterval_.second - valInterval_.first) / height();

	emit req_axisChange();
}

void wdgAxisValue::drawDashLines(QPainter& painter){

	int w = width(), h = height();

	painter.setPen(Qt::gray);
	painter.drawLine(QPoint(w-1, 0), QPoint(w-1, h));

	painter.setPen(Qt::black);
	int cHeight = curOffsPos_ % curDashStep_;

	while (cHeight < h){

		painter.drawLine(QPoint(w - cng_dashHeight_, cHeight), QPoint(w, cHeight));

		cHeight += curDashStep_;
	}
}

QString wdgAxisValue::getValMark(int offs){
		
	double vl = valInterval_.second - scale_ * offs;
			
	return QString::number(vl);
}

void wdgAxisValue::drawValMark(QPainter& painter){

	int w = width(), h = height();

	int cHeight = curOffsPos_ % curDashStep_;

	while (cHeight < h){

		QString valMark = getValMark(cHeight);
			
		painter.drawText(QPoint(0, cHeight), valMark);
				
		cHeight += curDashStep_;
				
	}
}

void wdgAxisValue::paintEvent(QPaintEvent *event){

	QPainter painter(this);

	drawDashLines(painter);

	drawValMark(painter);

}

void wdgAxisValue::setValInterval(double minv, double maxv){
	
	//minv = max(INT32_MIN, (int)minv); minv = min(INT32_MAX, (int)minv);
	//maxv = max(INT32_MIN, (int)maxv); maxv = min(INT32_MAX, (int)maxv);

	valInterval_.first = minv;
	valInterval_.second = maxv;

	curInterv_ = valInterval_.second - valInterval_.first;

	scale_ = (valInterval_.second - valInterval_.first) / height();
}

QPair<double, double> wdgAxisValue::getValInterval(){

	return valInterval_;

}

double wdgAxisValue::getValScale(){

	return scale_;
}

QVector<int> wdgAxisValue::getAxisMark(){

	QVector<int> mark;

	int h = height();

	int cHeight = curOffsPos_ % curDashStep_;

	while (cHeight < h){

		mark.push_back(cHeight);

		cHeight += curDashStep_;
	}

	return mark;

}