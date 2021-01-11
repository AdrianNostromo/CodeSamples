"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class InterfaceUtil {
    static implementsInterface(object, interfaceName) {
        return object.hasOwnProperty("_" + interfaceName);
    }
    ;
}
exports.InterfaceUtil = InterfaceUtil;
//# sourceMappingURL=InterfaceUtil.js.map