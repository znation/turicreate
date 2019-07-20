import * as React from 'react';
import { Component } from 'react';

import * as css from 'json!./index.css.json';

export default class Cell extends Component {
  render() {
      return (
        <div {...this.props} className={css['sticky-table-cell'] + ' ' + (this.props.className || '')}>
          {this.props.children}
        </div>
      );
  }
}
