import React, { Component } from 'react';
import * as css from 'json!./index.scss.json';

import TCEvaluationTooltip from '../TCEvaluationTooltip/index';

import info from 'image!./assets/info.svg';

class TCEvaluationMetrics extends Component {

  renderIcon = () => {
    if(this.props.tooltip){
      return (
        <div className={css.TCEvaluationMetricsInfoIcon}>
          <img src={info}
               height={10}
               width={10}/>
        </div>
      );
    }
  }

  renderTooltip = () => {
    if(this.props.tooltip){
      return (
        <div className={css.TCEvaluationMetricsTooltipContainer}>
          <TCEvaluationTooltip text={this.props.tooltip}/>
        </div>
      );
    }
  }

  render() {
    return (
      <div className={css.TCEvaluationMetrics}>
        <div className={css.TCEvaluationMetricsTitleContainer}>
          <div className={css.TCEvaluationMetricsTitle}>
            {this.props.title}
          </div>
          {this.renderIcon()}
          {this.renderTooltip()}
        </div>
        <div className={css.TCEvaluationMetricsValueContainer}>
          {this.props.value}
        </div>
      </div>
    );
  }
}

export default TCEvaluationMetrics;
