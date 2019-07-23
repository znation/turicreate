import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.scss.json';
import ClassNameMap from '../../../../../../util/ClassNameMap';
const css = ClassNameMap(_style_dict);

class TCEvaluationImageCellsHover extends Component {
  render() {
    return (
      <div className={css.TCEvaluationImageCellsHover}>
        <img width={200}
             src={this.props.src} />
      </div>
    );
  }
}

export default TCEvaluationImageCellsHover;
