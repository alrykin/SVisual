#pragma once

#include <string>
#include <vector>
#include <map>

#ifdef WIN32
#include <stdInt.h>
#endif

namespace SV_Cng {

/// тип польз переменной
	enum class valueType {
		tBool = 0,
		tInt = 1,
		tFloat = 2,
	};

/// значение польз переменной
	union value {
		bool tBool;
		int tInt;
		float tFloat;
	};

/// пакет записи
	struct recData {
		uint64_t beginTime;  ///< нач время, мс
		value *vals;         ///< данные

		recData() : beginTime(0), vals(nullptr) {};
	};

	/// сигнал
	struct signalData {

		bool isActive;          ///< активен
	    bool isDelete;          ///< удален
		bool isBuffEnable;      ///< буфер разрешен

		std::string name;       ///< имя
		std::string module;     ///< модуль
		std::string group;      ///< группа
		std::string comment;    ///< комментарий

		valueType type;         ///< тип

		recData lastData;       ///< последняя запись

		///// буфер данных
		std::vector<recData> buffData;         
		int buffBeginPos;
		int buffValuePos;
		uint64_t buffMinTime, buffMaxTime;
		double buffMinValue, buffMaxValue;
		///////

		signalData() {
			isActive = false;			
			isDelete = false;
			isBuffEnable = false;

			type = valueType::tBool;

			buffBeginPos = 0;
			buffValuePos = 0;
			buffMinTime = INT32_MAX;
			buffMaxTime = INT32_MIN;
			buffMinValue = 0;
			buffMaxValue = 0;
		}
	};

/// модуль
	struct moduleData {

		bool isActive;                ///< активен
		bool isEnable;                ///< разрешен
		bool isDelete;                ///< удален
		std::string module;             ///< название
		std::vector<std::string> signls;         ///< список сигналов

		/// модуль
		/// \param modul название модуля
		moduleData(std::string modul) {

			isActive = false;
			isEnable = false;
			isDelete = false;
			module = modul;
		}
	};

/// группа сигналов
	struct groupData {

		bool isActive;                     ///< активна

		std::string group;                 ///< имя

		std::vector<std::string> signls;   ///< список сигналов

		/// группа сигналов
		/// \param grp имя
		groupData(std::string grp) {
			isActive = true;
			group = grp;
		}
	};

/// тип события
	enum class eventType {
		none = -1,            ///< нет
		connectModule = 0,    ///< модуль подключен
		disconnectModule = 1, ///< модуль отключен
		less = 2,             ///< "<"
		equals = 3,           ///< "=="
		more = 4,             ///< ">"
		posFront = 5,         ///< положительный фронт
		negFront = 6,         ///< отрицательный фронт
	};

/// триггер
	struct triggerData {
		bool isActive;               ///< активен

		std::string name;            ///< название триггера
		std::string signal;          ///< сигнал
		std::string module;          ///< модуль

		eventType condType;          ///< тип
		int condValue;               ///< значение условия (порог срабатывания)
		int condTOut;                ///< таймаут срабатывания, с

		triggerData() {
			isActive = false;
			condType = eventType::none;
			condTOut = 0;
		}
	};

/// вернуть тип сигнала как значение
/// \param str
/// \return
	valueType getSVType(std::string str);

/// вернуть тип сигнала как строку
/// \param type
/// \return
	std::string getSVTypeStr(valueType type);

/// вернуть значение как строку
/// \param vt
/// \param value
/// \return
	std::string getSValue(valueType vt, double value);

/// вернуть тип события как строку
/// \param type
/// \return
	std::string getEventTypeStr(eventType type);

}