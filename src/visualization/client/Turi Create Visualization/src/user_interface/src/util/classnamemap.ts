const handler = {
    get: function(obj: ClassNameMap, prop: string) {
        if (prop in obj) {
            return obj[prop];
        }
        if (prop in obj._dict) {
            return obj._dict[prop];
        }
        throw("Missing property " + prop + " on object " + JSON.stringify(obj));
    }
};
class ClassNameMap {
    _dict: {string: string};
    constructor(dict: {string: string}) {
        this._dict = dict;
    }
}
export default function(dict: {string: string}) {
    const target = new ClassNameMap(dict);
    return new Proxy(target, handler);
}