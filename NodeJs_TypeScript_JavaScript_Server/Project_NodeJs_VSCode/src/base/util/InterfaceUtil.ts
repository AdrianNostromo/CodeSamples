export class InterfaceUtil {

    public static implementsInterface(object: Object, interfaceName: string): boolean {
        return object.hasOwnProperty("_" + interfaceName);
    };

}
