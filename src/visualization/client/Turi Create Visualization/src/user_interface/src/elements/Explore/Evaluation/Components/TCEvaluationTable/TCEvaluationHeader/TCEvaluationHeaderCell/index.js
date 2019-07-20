import * as React from 'react';
import { Component } from 'react';
import styles from 'json!./index.scss.json';

import down from 'image!./assets/down.svg';
import caret_down from 'image!./assets/caret-down.svg';

class TCEvaluationHeaderCell extends Component {

  renderCarret = () => {
    if(this.props.enabled){
      if(this.props.direction){
        return (
          <div className={styles.TCEvaluationHeaderCellDropDown}
               style={{"transform": "rotate(180deg)"}}>
            <img src={caret_down}/>
          </div>
        )
      }else{
        return (
          <div className={styles.TCEvaluationHeaderCellDropDown}>
            <img src={caret_down}/>
          </div>
        )
      }
    }else{
      return (
        <div className={styles.TCEvaluationHeaderCellDropDown}>
          <img src={down}/>
        </div>
      )
    }
  }

  render() {
    if(this.props.type == "images"){
      return (
        <div className={styles.TCEvaluationHeaderCellImages}>
          {this.props.name}
        </div>
      );
    }else if(this.props.type == "percent"){
      return (
        <div className={styles.TCEvaluationHeaderCellPercent}
             onClick={this.props.onclick.bind(this)}>
          {this.props.name}
          {this.renderCarret()}
        </div>
      );
    }else if(this.props.type == "amount"){
      return (
        <div className={styles.TCEvaluationHeaderCellAmount}
             onClick={this.props.onclick.bind(this)}>
          {this.props.name}
          {this.renderCarret()}
        </div>
      );
    }else{
      return (
        <div className={styles.TCEvaluationHeaderCellText}
             onClick={this.props.onclick.bind(this)}>
          {this.props.name}
          {this.renderCarret()}
        </div>
      );
    }

  }
}

export default TCEvaluationHeaderCell;
