import * as React from 'react';
import { Component } from 'react';

import * as _style_dict from 'json!./index.scss.json';
import ClassNameMap from '../../../../../util/ClassNameMap';
const css = ClassNameMap(_style_dict);

export default class Table extends Component {
  render() {
    return (
      <div {...this.props} className={css['sticky-table-container'] + ' ' + (this.props.className || '')}>
        {this.props.children}
      </div>
    );
  }
}
