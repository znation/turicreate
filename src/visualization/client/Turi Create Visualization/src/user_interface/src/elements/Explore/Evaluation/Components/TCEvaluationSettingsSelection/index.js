import React, { Component } from 'react';
import css from 'json!./index.css.json';

class TCEvaluationSettingsSelection extends Component {
  render() {
    return (
      <div className={css.TCEvaluationSettingsSelection}>
        <div className={css.TCEvaluationSettingsSelectionTitle}>
          Metrics
        </div>
        <div className={css.TCEvaluationSettingsSelectionCont}>
          <input type="checkbox"
                 name="accuracy_checkbox"
                 checked={this.props.accuracy}
                 onChange={this.props.changeAccuracy.bind(this)} />
          <span className={css.TCEvaluationSettingsSelectionContText}>
           Accuracy
          </span>
          <br/>

          <input type="checkbox"
                 name="f1_score_checkbox"
                 checked={this.props.f1_score}
                 onChange={this.props.changeF1Score.bind(this)} />
          <span className={css.TCEvaluationSettingsSelectionContText}>
           F1 Score
          </span>
          <br/>

          <input type="checkbox"
                 name="precision_checkbox"
                 checked={this.props.precision}
                 onChange={this.props.changePrecision.bind(this)} />
          <span className={css.TCEvaluationSettingsSelectionContText}>
           Precision
          </span>
          <br/>

          <input type="checkbox"
                 name="recall_checkbox"
                 checked={this.props.recall}
                 onChange={this.props.changeRecall.bind(this)} />
          <span className={css.TCEvaluationSettingsSelectionContText}>
           Recall
          </span>
        </div>
      </div>
    );
  }
}

export default TCEvaluationSettingsSelection;
