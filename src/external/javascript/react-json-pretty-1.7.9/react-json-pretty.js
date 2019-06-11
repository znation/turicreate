define("react-json-pretty", ["require", "exports", "create-react-class", "create-react-class", "react"], function (require, exports, crcStuff, create_react_class_1, react_1) {
    'use strict';
    Object.defineProperty(exports, "__esModule", { value: true });
    console.log('DEBUG: ', crcStuff, "END DEBUG");
    console.log('DEBUG2: ', create_react_class_1.createReactClass, "END DEBUG2");
    var _extends = Object.assign || function (target) { for (var i = 1; i < arguments.length; i++) {
        var source = arguments[i];
        for (var key in source) {
            if (Object.prototype.hasOwnProperty.call(source, key)) {
                target[key] = source[key];
            }
        }
    } return target; };
    function _objectWithoutProperties(obj, keys) { var target = {}; for (var i in obj) {
        if (keys.indexOf(i) >= 0)
            continue;
        if (!Object.prototype.hasOwnProperty.call(obj, i))
            continue;
        target[i] = obj[i];
    } return target; }
    exports.default = create_react_class_1.createReactClass({
        displayName: 'exports',
        _replace: function _replace(match, ind, key, val, tra) {
            var spanEnd = '</span>';
            var keySpan = '<span class=json-key>';
            var valSpan = '<span class=json-value>';
            var strSpan = '<span class=json-string>';
            var booSpan = '<span class=json-boolean>';
            var sps = ind || '';
            if (key) {
                sps = sps + '"' + keySpan + key.replace(/^"|":\s$/g, '') + spanEnd + '": ';
            }
            if (val) {
                if (val === 'true' || val === 'false') {
                    sps = sps + booSpan + val + spanEnd;
                }
                else {
                    sps = sps + (val[0] == '"' ? strSpan : valSpan) + val + spanEnd;
                }
            }
            return sps + (tra || '');
        },
        _pretty: function _pretty(obj, replacer, space) {
            var regLine = /^( *)("[^"]+": )?("[^"]*"|[\w.+-]*)?([,[{]|\[\s*\],?|\{\s*\},?)?$/mg;
            var text = JSON.stringify(obj, typeof replacer === 'function' ? replacer : null, isNaN(space) ? 2 : space);
            if (!text) {
                return text;
            }
            return text.replace(/&/g, '&amp;').replace(/\\"([^,])/g, '\\&quot;$1').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(regLine, this._replace);
        },
        render: function render() {
            var _props = this.props, json = _props.json, replacer = _props.replacer, space = _props.space, className = _props.className, themeClassName = _props.themeClassName, rest = _objectWithoutProperties(_props, ['json', 'replacer', 'space', 'className', 'themeClassName']);
            themeClassName = themeClassName ? themeClassName.trim() : themeClassName;
            className = className ? className.trim() : className;
            var themeClassNameFinal = themeClassName || 'json-pretty';
            var classNameFinal = className ? className + ' ' + themeClassNameFinal : themeClassNameFinal;
            if (typeof json === 'string') {
                try {
                    json = JSON.parse(json);
                }
                catch (e) {
                    console.error('The string is not a valid json data!', e);
                    return react_1.default.createElement('pre', _extends({}, rest, { className: classNameFinal || 'json-pretty', dangerouslySetInnerHTML: { __html: json } }));
                }
            }
            return react_1.default.createElement('pre', _extends({}, rest, { className: classNameFinal || 'json-pretty', dangerouslySetInnerHTML: { __html: this._pretty(json, replacer, +space) } }));
        }
    });
});
//# sourceMappingURL=react-json-pretty.js.map