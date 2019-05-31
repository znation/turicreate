import React, { Component } from 'react';
var style = require('json!./index.module.scss.json');

var error = require('image!./assets/error.svg');

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
        <img src={error} className={style.ErrorIcon}/>
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
