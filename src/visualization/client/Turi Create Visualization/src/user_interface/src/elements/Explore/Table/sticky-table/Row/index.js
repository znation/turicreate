import * as React from 'react';
import { Component } from 'react';

import * as css from 'json!../index.css.json';

export default class Row extends Component {
  render() {
    if(this.props.className == "accordian_window"){
      console.log(this.props);
    }
    return (
      <div {...this.props} className={css['sticky-table-row'] + ' ' + (this.props.className || '')}>
        {this.props.children}
      </div>
    );
  }
}
