import * as React from 'react';
import { Component } from 'react';
import * as css from 'json!./index.scss.json';

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
