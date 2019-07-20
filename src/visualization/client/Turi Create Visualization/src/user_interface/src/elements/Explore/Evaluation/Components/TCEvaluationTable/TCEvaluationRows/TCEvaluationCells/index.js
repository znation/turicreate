import React, { Component } from 'react';
import css from 'json!./index.scss.json';

import warning from 'image!./assets/warning.svg';
import TCEvaluationImageCells from '../../../TCEvaluationImageCells/index';

import * as d3 from "d3";

class TCEvaluationCells extends Component {
  constructor(props){
    super(props)
    this.formatDecimal = d3.format(".0f")
  }

  renderPercentLeft = () => {
    if(this.props.value >= 0.5){
      return (
        <div className={css.TCEvaluationCellsPercentShadedText}
             style={{"color": "#FFFFFF"}}>
          {this.formatDecimal(this.props.value*100)+"%"}
        </div>
      );
    }
  }

  renderAmountLeft = () => {
    if((this.props.value/this.props.max) >= 0.5){
      return (
        <div className={css.TCEvaluationCellsPercentShadedText}
             style={{"color": "#FFFFFF"}}>
          {this.formatDecimal(this.props.value)}
        </div>
      );
    }
  }

  renderPercentRight = () => {
    if(this.props.value < 0.5){
      return (
        <div className={css.TCEvaluationCellsPercentShadedText}
             style={{"color": "#484856"}}> 
          {this.formatDecimal(this.props.value*100)+"%"}
        </div>
      );
    }
  }

  renderAmountRight = () => {
    if((this.props.value/this.props.max) < 0.5){
      return (
        <div className={css.TCEvaluationCellsPercentShadedText}
             style={{"color": "#484856"}}>
          {this.formatDecimal(this.props.value)}
        </div>
      );
    }
  }

  renderWarning = () => {
    if(this.props.value <= 0.2){
      return (
        <div className={css.TCEvaluationCellsPercentShadedWarning}>
          <img src={warning} height={16}/>
        </div>
      );
    }
  }

  render() {
    if(this.props.type == "images"){
      return (
        <div className={css.TCEvaluationCellsImages}>
          <div className={css.TCEvaluationCellsImagesContainer}>
           {this.props.value.slice(0, 20).map((data, index) => {
            return(
              <TCEvaluationImageCells value={data}/>
            )
           })}
          </div>
        </div>
       );
    }else if(this.props.type == "percent"){
      return (
        <div className={css.TCEvaluationCellsPercent}>
          <div className={css.TCEvaluationCellsPercentShaded}
               style={{"width": this.props.value*100+"%"}}>
               { this.renderPercentLeft() }
          </div>
          {this.renderPercentRight()}
          {this.renderWarning()}
        </div>
      );
    }else if(this.props.type == "amount"){
      return (
        <div className={css.TCEvaluationCellsAmount}>
          <div className={css.TCEvaluationCellsAmountShaded}
               style={{"width": (this.props.value/this.props.max)*100+"%"}}>
               { this.renderAmountLeft() }
          </div>
          {this.renderAmountRight()}
        </div>
      );
    }else{
      return (
        <div className={css.TCEvaluationCellsText}>
          <div className={css.TCEvaluationCellsTextCont}>
            {this.props.value}
          </div>
        </div>
      );
    }
  }
}

export default TCEvaluationCells;
