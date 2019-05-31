import React, { Component } from 'react';
var styles = require('json!./index.scss.json');

import * as d3 from "d3";

import TCEvaluationImageCells from '../../../TCEvaluationImageCells/index.js';

class TCEvaluationConfusionCell extends Component {
	constructor(props){
    super(props)
    this.formatDecimal = d3.format(".0f") 
  }

	renderAmountLeft = () => {
    if((this.props.value/this.props.max) >= 0.5){
      return (
        <div className={styles.TCEvaluationCellsPercentShadedText}
             style={{"color": "#FFFFFF"}}>
          {this.formatDecimal(this.props.value)}
        </div>
      );
    }
  }

  renderAmountRight = () => {
    if((this.props.value/this.props.max) < 0.5){
      return (
        <div className={styles.TCEvaluationCellsPercentShadedText}
             style={{"color": "#484856"}}>
          {this.formatDecimal(this.props.value)}
        </div>
      );
    }
  }

  render() {
  	if(this.props.type == "images"){
      return (
        <div className={styles.TCEvaluationConfusionCellImage}>
          {this.props.value.map((data, index) => {
            return(
              <TCEvaluationImageCells value={data}/>
            )
          })}
        </div>
      );
    }else if(this.props.type == "percent"){
      return (
        <div className={styles.TCEvaluationConfusionCell}>
         
        </div>
      );
    }else if(this.props.type == "amount"){
      return (
        <div className={styles.TCEvaluationCellsAmount}>
          <div className={styles.TCEvaluationCellsAmountShaded}
               style={{"width": (this.props.value/this.props.max)*100+"%"}}>
               { this.renderAmountLeft() }
          </div>
          {this.renderAmountRight()}
        </div>
      );
    }else{
      return (
        <div className={styles.TCEvaluationConfusionCellText}>
        	{this.props.value}
        </div>
      );
  	}
  }
}

export default TCEvaluationConfusionCell;
