import React, { Component } from 'react';
import ReactDOM from 'react-dom';

import * as css from 'json!./index.css.json';

export default class Table extends Component {
  render() {
    return (
      <div {...this.props} className={css['sticky-table-container'] + ' ' + (this.props.className || '')}>
        {this.props.children}
      </div>
    );
  }
}
