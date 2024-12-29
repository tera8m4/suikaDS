import { customElement, property, query, state } from "lit/decorators.js";
import { Element } from "../element";
import { html } from "lit";
import { animate } from '@lit-labs/motion';
import { Config } from "../../config";

@customElement("anki-settings")
export class AnkiSettings extends Element {
  @property({ type: Boolean })
  isOpen: boolean = false;

  @property({ type: Function })
  onClose: () => void = () => { };

  @state()
  pictureFieldName: string = "";

  @query('#picture-field')
  input!: HTMLInputElement;

  @state()
  isShaking: boolean = false;

  constructor() {
    super();
    this.pictureFieldName = Config.getInstance().getData().ankiPictureField;
  }

  private async onSave() {
    this.input.disabled = true;

    this.isShaking = true;

    const data = Config.getInstance().getData();
    data.ankiPictureField = this.pictureFieldName;
    Config.getInstance().update(data);

    setTimeout(() => {
      this.isShaking = false;
      this.onCloseAction();
    }, 400);
  }

  private onCloseAction(): void {
    this.input.disabled = false;
    this.dispatchEvent(new CustomEvent('onClose'));
  }

  private get animationOptions() {
    return {
      keyframeOptions: { duration: 600, easing: 'ease-out' },
      properties: ['transform'],
      in: [
        { transform: 'scale(0.9)' },
        { transform: 'scale(1.2)' },
        { transform: 'scale(1)' },
      ],
      out: [
        { transform: 'scale(1)' },
        { transform: 'scale(0.9)' },
      ],
      stabilizeOut: true,
      guard: () => this.isShaking,
    };
  }

  onPictureFieldChange(event: any) {
    this.pictureFieldName = event.target.value;
  }

  onSubmit(event: any) {
    event.preventDefault();
  }

  protected render() {
    return html`<dialog ?open=${this.isOpen}>
      <article class=${this.isShaking ? animate(this.animationOptions) : ""}">
        <header>
          <button aria-label="Close" rel="prev" @click=${this.onCloseAction}></button>
          <h2>Anki settings</h2>
        </header>
        <form @submit=${this.onSubmit}>
          <fieldset>
            <label>Picture field
            <input name="picture" id="picture-field" placeholder="Anki deck picture field name" .value=${this.pictureFieldName} @input=${this.onPictureFieldChange}/>
            </label>            
          </fieldset>
        </form>
        <footer>
          <button class="secondary" @click=${this.onCloseAction}>Cancel</button>
          <button @click=${this.onSave}>Save</button>
        </footer>
      </article>
    </dialog>`
  }
}
