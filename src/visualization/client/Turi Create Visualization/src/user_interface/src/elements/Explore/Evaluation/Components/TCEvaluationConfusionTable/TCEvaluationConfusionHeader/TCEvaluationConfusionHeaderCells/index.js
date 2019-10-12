import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.scss.json';
import ClassNameMap from '../../../../../../../util/ClassNameMap';
const styles = ClassNameMap(_style_dict);

import down from 'image!./assets/down.svg';
import caret_down from 'image!./assets/caret-down.svg';

class TCEvaluationConfusionHeaderCell extends Component {

  renderCaret = () => {
    if(this.props.enabled){
      if(this.props.direction){
        return (
          <div className={styles.TCEvaluationConfusionHeaderCellDropDown}
               style={{"transform": "rotate(180deg)"}}>
            <img src={caret_down}
		 alt=""/>
          </div>
        )
      }else{
        return (
          <div className={styles.TCEvaluationConfusionHeaderCellDropDown}>
            <img src={caret_down} alt="" />
          </div>
        )
      }
    }else{
      return (
        <div className={styles.TCEvaluationConfusionHeaderCellDropDown}>
          <img src={down} alt="" />
        </div>
      )
    }
  }

  render() {
    if(this.props.type === "images"){
      return (
        <div className={styles.TCEvaluationConfusionHeaderCellImages}>
          {this.props.name}
        </div>
      );
    }else if(this.props.type === "percent"){
      return (
        <div className={styles.TCEvaluationConfusionHeaderCellPercent}
             onClick={this.props.onclick.bind(this)}>
          {this.props.name}
          {this.renderCaret()}
        </div>
      );
    }else if(this.props.type === "amount"){
      return (
        <div className={styles.TCEvaluationConfusionHeaderCellAmount}
             onClick={this.props.onclick.bind(this)}>
          {this.props.name}
          {this.renderCaret()}
        </div>
      );
    }else{
      return (
        <div className={styles.TCEvaluationConfusionHeaderCellText}
             onClick={this.props.onclick.bind(this)}>
          {this.props.name}
          {this.renderCaret()}
        </div>
      );
    }
  }
}

export default TCEvaluationConfusionHeaderCell;
