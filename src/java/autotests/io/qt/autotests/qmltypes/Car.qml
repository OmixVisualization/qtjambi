import QtQuick 2.0
import io.qt.autotests.qmltypes 5.0

QtObject {
	property real wheelRadius: 52
	property Engine engine: Engine{ performance: 150 }
	property list<Wheel> wheels: [
							Wheel{ radius: wheelRadius },
							Wheel{ radius: wheelRadius },
							Wheel{ radius: wheelRadius },
							Wheel{ radius: wheelRadius }
						  ]
}