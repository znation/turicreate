import * as React from 'react';
import { Component } from 'react';
import styles from 'json!./index.module.scss.json';

class ProgressBar extends Component {
  render() {
    const css_gradient = {
      "background": "-webkit-linear-gradient(left, #3b99fc 0%,#3b99fc "+parseInt(this.props.percent, 10)+"%,#ededeb "+parseInt(this.props.percent, 10)+"%,#ededeb 100%)",
      "background": "linear-gradient(to right, #3b99fc 0%,#3b99fc "+parseInt(this.props.percent, 10)+"%,#ededeb "+parseInt(this.props.percent, 10)+"%,#ededeb 100%)"
    };

    return (
      <div style={css_gradient} className={styles.ProgressBar}>
      </div>
    );
  }
}

export default ProgressBar;