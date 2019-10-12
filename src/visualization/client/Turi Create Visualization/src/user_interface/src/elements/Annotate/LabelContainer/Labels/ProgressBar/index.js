import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.module.scss.json';
import ClassNameMap from '../../../../../util/ClassNameMap';
const styles = ClassNameMap(_style_dict);

class ProgressBar extends Component {
  render() {
    const css_gradient = {
      "_background": "-webkit-linear-gradient(left, #3b99fc 0%,#3b99fc "+parseInt(this.props.percent, 10)+"%,#ededeb "+parseInt(this.props.percent, 10)+"%,#ededeb 100%)",
      "background": "linear-gradient(to right, #3b99fc 0%,#3b99fc "+parseInt(this.props.percent, 10)+"%,#ededeb "+parseInt(this.props.percent, 10)+"%,#ededeb 100%)"
    };

    return (
      <div style={css_gradient} className={styles.ProgressBar}>
      </div>
    );
  }
}

export default ProgressBar;
