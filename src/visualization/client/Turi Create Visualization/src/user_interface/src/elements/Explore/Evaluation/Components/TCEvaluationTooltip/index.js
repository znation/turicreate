import * as React from 'react';
import { Component } from 'react';
import * as css from 'json!./index.scss.json';

import carret from 'image!./assets/carret.svg';

class TCEvaluationTooltip extends Component {
  render() {
    return (
      <div className={css.TCEvaluationTooltip}>
        <div className={css.TCEvaluationTooltipText}>
          {this.props.text}
        </div>
        <div className={css.TCEvaluationCarret}>
          <img src={carret}
               height={9}
               className={css.TCEvaluationCarretIcon}/>
        </div>
      </div>
    );
  }
}

export default TCEvaluationTooltip;
