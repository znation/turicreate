import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.module.scss.json';
import ClassNameMap from '../../../util/ClassNameMap';
const style = ClassNameMap(_style_dict);
  
class ToggleButton extends Component {
  render() {
    var NibClasses = style.ToggleButtonNib
    var BackgroundClasses = style.ToggleButton

    if(this.props.hideAnnotated){
      NibClasses = style.Toggled
      BackgroundClasses = style.ToggleButtonOn
    }

    return (
      <div className={BackgroundClasses}
           onClick={this.props.toggleHideAnnotated.bind(this)}>
        <div className={NibClasses}
             onClick={this.props.toggleHideAnnotated.bind(this)}>
        </div>
      </div>
    );
  }
}

export default ToggleButton;
