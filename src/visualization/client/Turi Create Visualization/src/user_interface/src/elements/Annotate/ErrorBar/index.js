import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.module.scss.json';
import ClassNameMap from '../../../util/ClassNameMap';
const style = ClassNameMap(_style_dict);

import error from 'image!./assets/error.svg';

/* TODO: Design Better Error Message Compornent */
class ErrorBar extends Component {
  constructor(props){
    super(props);
    this.state = {
    }
  }
  render() {
    return (
      <div className={style.ErrorBar}>
        <img src={error} className={style.ErrorIcon} alt=""/>
        <div className={style.ErrorMessage}>
          {this.props.message}
        </div>
        <div className={style.ErrorClose}
             onClick={this.props.clearError}>
          close
        </div>
      </div>
    );
  }
}

export default ErrorBar;
